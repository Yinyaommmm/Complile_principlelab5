#include "ssa.h"
#include <cassert>
#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "bg_llvm.h"
#include "graph.hpp"
#include "liveness.h"
#include "printLLVM.h"

using namespace std;
using namespace LLVMIR;
using namespace GRAPH;
struct imm_Dominator {
    LLVMIR::L_block* pred;
    unordered_set<LLVMIR::L_block*> succs;
};

unordered_map<L_block*, unordered_set<L_block*>> dominators;
unordered_map<L_block*, imm_Dominator> tree_dominators;
unordered_map<L_block*, unordered_set<L_block*>> DF_array;
unordered_map<L_block*, Node<LLVMIR::L_block*>*> revers_graph;
unordered_map<Temp_temp*, AS_operand*> temp2ASoper;

static void init_table() {
    dominators.clear();
    tree_dominators.clear();
    DF_array.clear();
    revers_graph.clear();
    temp2ASoper.clear();
}

LLVMIR::L_prog* SSA(LLVMIR::L_prog* prog) {
    int i = 0;
    for (auto& fun : prog->funcs) {
        printf("-------------ssa : block %d-------------\n", i++);
        init_table();
        // AS_operand* 指向同一块 AS_operand 实例
        combine_addr(fun);
        mem2reg(fun);
        auto RA_bg = Create_bg(fun->blocks);
        SingleSourceGraph(RA_bg.mynodes[0], RA_bg, fun);
        // Show_graph(stdout,RA_bg);
        Liveness(RA_bg.mynodes[0], RA_bg, fun->args);
        Dominators(RA_bg);
        printf_domi();
        // tree_Dominators(RA_bg);
        // // printf_D_tree();
        // // 默认0是入口block
        // computeDF(RA_bg, RA_bg.mynodes[0]);
        // // printf_DF();
        // Place_phi_fu(RA_bg, fun);
        // Rename(RA_bg);
        // combine_addr(fun);
    }
    return prog;
}

static bool is_mem_variable(L_stm* stm) {
    return stm->type == L_StmKind::T_ALLOCA &&
           stm->u.ALLOCA->dst->kind == OperandKind::TEMP &&
           stm->u.ALLOCA->dst->u.TEMP->type == TempType::INT_PTR &&
           stm->u.ALLOCA->dst->u.TEMP->len == 0;
}

// 保证相同的AS_operand,地址一样 。常量除外
void combine_addr(LLVMIR::L_func* fun) {
    unordered_map<Temp_temp*, unordered_set<AS_operand**>> temp_set;
    unordered_map<Name_name*, unordered_set<AS_operand**>> name_set;
    for (auto& block : fun->blocks) {
        for (auto& stm : block->instrs) {
            // 获取ir句子里的AS operand**
            auto AS_operand_list = get_all_AS_operand(stm);
            for (auto AS_op : AS_operand_list) {
                // 记录变量对应的AS_oprand**
                if ((*AS_op)->kind == OperandKind::TEMP) {
                    temp_set[(*AS_op)->u.TEMP].insert(AS_op);
                } else if ((*AS_op)->kind == OperandKind::NAME) {
                    name_set[(*AS_op)->u.NAME].insert(AS_op);
                }
            }
        }
    }
    for (auto temp : temp_set) {
        // 所有同一个变量的AS_oprand
        AS_operand* fi_AS_op = **temp.second.begin();
        for (auto AS_op : temp.second) {
            *AS_op = fi_AS_op;
        }
    }
    for (auto name : name_set) {
        AS_operand* fi_AS_op = **name.second.begin();
        for (auto AS_op : name.second) {
            *AS_op = fi_AS_op;
        }
    }
}

void mem2reg(LLVMIR::L_func* fun) {
    //   Todo

    unordered_map<AS_operand*, AS_operand*> nameCover;
    // nameCover[a] = b,说明a寄存器的位置可用b寄存器替代
    // nameCover[a] = nullptr ，说明a寄存器已经是根，无法替代

    // 寻找标量，并且记录寄存器序号覆盖关系
    int n = 0;
    for (L_block* block : fun->blocks) {
        list<L_stm*>& instrs = block->instrs;
        for (auto it = instrs.begin(); it != instrs.end();) {
            L_stm* stm = *it;
            if (stm->type == L_StmKind::T_ALLOCA) {
                // 直接删除,it指向下一个
                if (is_mem_variable(stm)) {
                    AS_operand* dst = stm->u.ALLOCA->dst;
                    AS_operand* int0 = AS_Operand_Const(0);
                    nameCover[stm->u.ALLOCA->dst] = nullptr;
                    it = instrs.erase(it);
                    it = instrs.insert(it, L_Move(int0, dst));
                    // 移到下一条指令
                    it++;
                } else {
                    it++;
                }
            } else if (stm->type == L_StmKind::T_LOAD) {
                // dst = load from ptr
                AS_operand *dst = stm->u.LOAD->dst, *ptr = stm->u.LOAD->ptr;
                // 如果ptr是个标量，需要删除。
                if (isScalar_Alloca(ptr)) {
                    nameCover[dst] = ptr;
                    it = instrs.erase(it);
                } else {
                    it++;
                }
            } else if (stm->type == L_StmKind::T_STORE) {
                // store i32 %src, i32* %ptr
                L_store* store = stm->u.STORE;
                AS_operand *src = store->src, *ptr = store->ptr;
                if (!isScalar_Alloca(ptr)) {
                    it++;
                    continue;
                }
                if (src->kind == OperandKind::ICONST) {
                    // 删除store
                    it = instrs.erase(it);
                    // 添加move,
                    it = instrs.insert(it, L_Move(src, ptr));
                    // 移到下一条指令
                    it++;
                } else if (src->kind == OperandKind::TEMP) {
                    nameCover[src] = ptr;
                    // 自动移动到下一条指令
                    it = instrs.erase(it);
                    // 添加move,
                    it = instrs.insert(it, L_Move(src, ptr));
                    // 移到下一条指令
                    it++;
                } else {
                    // 不可能是全局变量store进来
                    assert(0);
                }
            } else {
                it++;
            }
        }
    }

    // 名字更改
    for (L_block* block : fun->blocks) {
        list<L_stm*>& instrs = block->instrs;
        for (auto it = instrs.begin(); it != instrs.end(); it++) {
            L_stm* stm = *it;
            switch (stm->type) {
                case L_StmKind::T_BINOP: {
                    L_binop* binop = stm->u.BINOP;
                    AS_operand *left = binop->left, *right = binop->right,
                               *dst = binop->dst;
                    cover(left, nameCover);
                    cover(right, nameCover);
                    cover(dst, nameCover);
                } break;
                case L_StmKind::T_LOAD:
                case L_StmKind::T_LABEL:
                case L_StmKind::T_JUMP:
                    break;
                case L_StmKind::T_STORE: {
                    L_store* store = stm->u.STORE;
                    // src可以替换
                    cover(store->src, nameCover);
                } break;
                case L_StmKind::T_CMP: {
                    L_cmp* cmp = stm->u.CMP;
                    AS_operand *left = cmp->left, *right = cmp->right,
                               *dst = cmp->dst;
                    cover(left, nameCover);
                    cover(right, nameCover);
                    cover(dst, nameCover);
                } break;
                case L_StmKind::T_CJUMP: {
                    cover(stm->u.CJUMP->dst, nameCover);
                } break;
                case L_StmKind::T_MOVE: {
                    cover(stm->u.MOVE->src, nameCover);
                    cover(stm->u.MOVE->dst, nameCover);
                } break;
                case L_StmKind::T_CALL: {
                    cover(stm->u.CALL->res, nameCover);
                    for (AS_operand*& arg : stm->u.CALL->args) {
                        cover(arg, nameCover);
                    }
                } break;
                case L_StmKind::T_VOID_CALL: {
                    for (AS_operand*& arg : stm->u.VOID_CALL->args) {
                        cover(arg, nameCover);
                    }
                } break;
                case L_StmKind::T_RETURN: {
                    cover(stm->u.RET->ret, nameCover);
                } break;
                case L_StmKind::T_PHI:
                case L_StmKind::T_NULL:
                case L_StmKind::T_ALLOCA:
                    break;
                case L_StmKind::T_GEP: {
                    cover(stm->u.GEP->base_ptr, nameCover);
                    cover(stm->u.GEP->index, nameCover);
                    cover(stm->u.GEP->new_ptr, nameCover);
                } break;
                default:
                    break;
            }
        }
    }
}

bool DominatorIteration(GRAPH::Node<LLVMIR::L_block*>* r,
                        GRAPH::Graph<LLVMIR::L_block*>& bg,
                        int iterTimes) {
    int dyeColor = 99999 + iterTimes;
    // 为自身染色
    r->color = dyeColor;

    // 创建新Dom
    unordered_set<L_block*> newDom;
    newDom.emplace(r->info);
    // 得到前驱的交集
    unordered_set<L_block*> predIntersection;
    for (int predId : r->preds) {
        GRAPH::Node<LLVMIR::L_block*>* node = bg.mynodes[predId];
        predIntersection =
            dom_intersection(predIntersection, dominators[node->info]);
    }
    // 得到新Dom的完全体
    newDom = dom_union(newDom, predIntersection);
    // 判断自身是否发生变化
    bool needMoreIter = false;
    unordered_set<L_block*>& curDom = dominators[r->info];
    needMoreIter = needMoreIter || (curDom != newDom);  // set直接用==判断

    // 深搜遍历其余节点
    for (int succId : r->succs) {
        GRAPH::Node<LLVMIR::L_block*>* node = bg.mynodes[succId];
        if (node->color != dyeColor) {
            needMoreIter =
                needMoreIter || DominatorIteration(node, bg, iterTimes);
        }
    }

    return needMoreIter;
}

void Dominators(GRAPH::Graph<LLVMIR::L_block*>& bg) {
    // 添加了DominatorIteration
    // 添加了DomUnion DomIntersection
    // 修改了Dominators ssa.cpp
    // 最好添加clearColors graph.hpp

    // 清除颜色
    bg.clearColor();

    GRAPH::Node<LLVMIR::L_block*>* srcNode = bg.mynodes[0];
    // 初始化dominators
    dominators[srcNode->info].emplace(srcNode->info);
    // 全部节点的集合
    unordered_set<L_block*> allBlock;
    for (auto it = bg.mynodes.begin(); it != bg.mynodes.end(); it++) {
        allBlock.emplace(it->second->info);
    }
    for (auto it = bg.mynodes.begin(); it != bg.mynodes.end(); it++) {
        // 其余节点必须包含所有节点。
        if (dominators[it->second->info].size() == 0) {
            dominators[it->second->info] = allBlock;
        }
    }

    // 迭代更新
    bool changed = true;
    int iterTimes = 0;
    while (changed) {
        changed = DominatorIteration(srcNode, bg, iterTimes);
        iterTimes++;
    }
    printf_domi();
}

void printf_domi() {
    printf("Dominator:\n");
    for (auto x : dominators) {
        printf("%s :\n", x.first->label->name.c_str());
        for (auto t : x.second) {
            printf("%s ", t->label->name.c_str());
        }
        printf("\n\n");
    }
}

void printf_D_tree() {
    printf("dominator tree:\n");
    for (auto x : tree_dominators) {
        printf("%s :\n", x.first->label->name.c_str());
        for (auto t : x.second.succs) {
            printf("%s ", t->label->name.c_str());
        }
        printf("\n\n");
    }
}

void printf_DF() {
    printf("DF:\n");
    for (auto x : DF_array) {
        printf("%s :\n", x.first->label->name.c_str());
        for (auto t : x.second) {
            printf("%s ", t->label->name.c_str());
        }
        printf("\n\n");
    }
}

void tree_Dominators(GRAPH::Graph<LLVMIR::L_block*>& bg) {
    //   Todo
}

void computeDF(GRAPH::Graph<LLVMIR::L_block*>& bg,
               GRAPH::Node<LLVMIR::L_block*>* r) {
    //   Todo
}

// 只对标量做
void Place_phi_fu(GRAPH::Graph<LLVMIR::L_block*>& bg, L_func* fun) {
    //   Todo
}

static list<AS_operand**> get_def_int_operand(LLVMIR::L_stm* stm) {
    list<AS_operand**> ret1 = get_def_operand(stm), ret2;
    for (auto AS_op : ret1) {
        if ((**AS_op).u.TEMP->type == TempType::INT_TEMP) {
            ret2.push_back(AS_op);
        }
    }
    return ret2;
}

static list<AS_operand**> get_use_int_operand(LLVMIR::L_stm* stm) {
    list<AS_operand**> ret1 = get_use_operand(stm), ret2;
    for (auto AS_op : ret1) {
        if ((**AS_op).u.TEMP->type == TempType::INT_TEMP) {
            ret2.push_back(AS_op);
        }
    }
    return ret2;
}

static void Rename_temp(GRAPH::Graph<LLVMIR::L_block*>& bg,
                        GRAPH::Node<LLVMIR::L_block*>* n,
                        unordered_map<Temp_temp*, stack<Temp_temp*>>& Stack) {
    //   Todo
}

void Rename(GRAPH::Graph<LLVMIR::L_block*>& bg) {
    //   Todo
}

bool isScalar_Alloca(AS_operand* a) {
    return a && a->kind == OperandKind::TEMP &&
           a->u.TEMP->type == TempType::INT_PTR && a->u.TEMP->len == 0;
}
// mem2reg中名字覆盖(temp_temp改至一样)
void cover(AS_operand* cur,
           std::unordered_map<AS_operand*, AS_operand*>& nameCover) {
    // 只翻译标量
    if (!cur || cur->kind != OperandKind::TEMP ||
        cur->u.TEMP->type != TempType::INT_TEMP) {
        return;
    }
    if (nameCover.find(cur) == nameCover.end()) {
        return;
    }
    // 递归查找到最深处的Asoprand
    AS_operand* root = nameCover[cur];
    if (root == nullptr) {
        return;  // 本身就是根元素，无需覆盖,别人得跟他叫一个名
    }
    int level = 0;
    while (nameCover[root] != nullptr) {
        root = nameCover[root];
    }
    cur->u.TEMP = root->u.TEMP;
}

// Dom的并
unordered_set<L_block*> dom_union(unordered_set<L_block*>& a,
                                  unordered_set<L_block*>& b) {
    unordered_set<L_block*> res;
    for (auto block : a) {
        res.emplace(block);
    }
    for (auto block : b) {
        res.emplace(block);
    }
    return res;
}
// Dom的交
unordered_set<L_block*> dom_intersection(unordered_set<L_block*>& a,
                                         unordered_set<L_block*>& b) {
    unordered_set<L_block*> res;
    for (auto block : a) {
        if (b.find(block) != b.end()) {
            res.emplace(block);
        }
    }
    return res;
}
