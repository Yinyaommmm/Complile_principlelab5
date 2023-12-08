#include "liveness.h"
#include <unordered_map>
#include <unordered_set>
#include "graph.hpp"
#include "llvm_ir.h"
#include "temp.h"

using namespace std;
using namespace LLVMIR;
using namespace GRAPH;

struct inOut {
    TempSet_ in;
    TempSet_ out;
};

struct useDef {
    TempSet_ use;
    TempSet_ def;
};

static unordered_map<GRAPH::Node<LLVMIR::L_block*>*, inOut> InOutTable;
static unordered_map<GRAPH::Node<LLVMIR::L_block*>*, useDef> UseDefTable;

list<AS_operand**> get_all_AS_operand(L_stm* stm) {
    list<AS_operand**> AS_operand_list;
    switch (stm->type) {
        case L_StmKind::T_BINOP: {
            AS_operand_list.push_back(&(stm->u.BINOP->left));
            AS_operand_list.push_back(&(stm->u.BINOP->right));
            AS_operand_list.push_back(&(stm->u.BINOP->dst));

        } break;
        case L_StmKind::T_LOAD: {
            AS_operand_list.push_back(&(stm->u.LOAD->dst));
            AS_operand_list.push_back(&(stm->u.LOAD->ptr));
        } break;
        case L_StmKind::T_STORE: {
            AS_operand_list.push_back(&(stm->u.STORE->src));
            AS_operand_list.push_back(&(stm->u.STORE->ptr));
        } break;
        case L_StmKind::T_LABEL: {
        } break;
        case L_StmKind::T_JUMP: {
        } break;
        case L_StmKind::T_CMP: {
            AS_operand_list.push_back(&(stm->u.CMP->left));
            AS_operand_list.push_back(&(stm->u.CMP->right));
            AS_operand_list.push_back(&(stm->u.CMP->dst));
        } break;
        case L_StmKind::T_CJUMP: {
            AS_operand_list.push_back(&(stm->u.CJUMP->dst));
        } break;
        case L_StmKind::T_MOVE: {
            AS_operand_list.push_back(&(stm->u.MOVE->src));
            AS_operand_list.push_back(&(stm->u.MOVE->dst));
        } break;
        case L_StmKind::T_CALL: {
            AS_operand_list.push_back(&(stm->u.CALL->res));
            for (auto& arg : stm->u.CALL->args) {
                AS_operand_list.push_back(&arg);
            }
        } break;
        case L_StmKind::T_VOID_CALL: {
            for (auto& arg : stm->u.VOID_CALL->args) {
                AS_operand_list.push_back(&arg);
            }
        } break;
        case L_StmKind::T_RETURN: {
            if (stm->u.RET->ret != nullptr)
                AS_operand_list.push_back(&(stm->u.RET->ret));
        } break;
        case L_StmKind::T_PHI: {
            AS_operand_list.push_back(&(stm->u.PHI->dst));
            for (auto& phi : stm->u.PHI->phis) {
                AS_operand_list.push_back(&(phi.first));
            }
        } break;
        case L_StmKind::T_ALLOCA: {
            AS_operand_list.push_back(&(stm->u.ALLOCA->dst));
        } break;
        case L_StmKind::T_GEP: {
            AS_operand_list.push_back(&(stm->u.GEP->new_ptr));
            AS_operand_list.push_back(&(stm->u.GEP->base_ptr));
            AS_operand_list.push_back(&(stm->u.GEP->index));
        } break;
        default: {
            printf("%d\n", (int)stm->type);
            assert(0);
        }
    }
    return AS_operand_list;
}

void append(std::list<AS_operand**>& list, AS_operand*& value) {
    if (value == nullptr) {
        return;
    }
    if (value->kind != OperandKind::TEMP) {
        return;
    }
    list.push_back(&value);
}

std::list<AS_operand**> get_def_operand(L_stm* stm) {
    //   Todo
    list<AS_operand**> defList;
    switch (stm->type) {
        case L_StmKind::T_BINOP: {
            append(defList, (stm->u.BINOP->dst));
        } break;
        case L_StmKind::T_LOAD: {
            append(defList, (stm->u.LOAD->dst));
        } break;
        case L_StmKind::T_CMP: {
            append(defList, (stm->u.CMP->dst));
        } break;
        case L_StmKind::T_MOVE: {
            append(defList, (stm->u.MOVE->dst));
        } break;
        case L_StmKind::T_CALL: {
            append(defList, (stm->u.CALL->res));
        } break;
        case L_StmKind::T_PHI: {
            append(defList, (stm->u.PHI->dst));
        } break;
        case L_StmKind::T_ALLOCA: {
            append(defList, (stm->u.ALLOCA->dst));
        } break;
        case L_StmKind::T_GEP: {
            append(defList, (stm->u.GEP->new_ptr));
        } break;
        default: {
        }
    }
    return defList;
}

// 获得语句中左值变量（定义的变量）
list<Temp_temp*> get_def(L_stm* stm) {
    auto AS_operand_list = get_def_operand(stm);
    list<Temp_temp*> Temp_list;
    for (auto AS_op : AS_operand_list) {
        Temp_list.push_back((*AS_op)->u.TEMP);
    }
    return Temp_list;
}

// 获得语句中使用的变量
std::list<AS_operand**> get_use_operand(L_stm* stm) {
    std::list<AS_operand**> useList;
    switch (stm->type) {
        case L_StmKind::T_BINOP:
            append(useList, stm->u.BINOP->left);
            append(useList, stm->u.BINOP->right);
            break;

        case L_StmKind::T_CALL:
            for (AS_operand*& arg : stm->u.CALL->args) {
                append(useList, arg);
            }
            break;

        case L_StmKind::T_CJUMP:
            append(useList, stm->u.CJUMP->dst);
            break;

        case L_StmKind::T_CMP:
            append(useList, stm->u.CMP->left);
            append(useList, stm->u.CMP->right);
            break;

        case L_StmKind::T_GEP:
            append(useList, stm->u.GEP->base_ptr);
            break;

        case L_StmKind::T_LOAD:
            append(useList, stm->u.LOAD->ptr);
            break;

        case L_StmKind::T_MOVE:
            append(useList, stm->u.MOVE->src);
            break;

        case L_StmKind::T_RETURN:
            append(useList, stm->u.RET->ret);
            break;

        case L_StmKind::T_STORE:
            append(useList, stm->u.STORE->src);
            break;

        case L_StmKind::T_VOID_CALL:
            for (AS_operand*& arg : stm->u.VOID_CALL->args) {
                append(useList, arg);
            }
            break;

        default:
            break;
    }
    return useList;
}

// 拿到一条命令中使用的寄存器(编号)
list<Temp_temp*> get_use(L_stm* stm) {
    auto AS_operand_list = get_use_operand(stm);
    list<Temp_temp*> Temp_list;
    for (auto AS_op : AS_operand_list) {
        Temp_list.push_back((*AS_op)->u.TEMP);
    }
    return Temp_list;
}

static void init_INOUT() {
    InOutTable.clear();
    UseDefTable.clear();
}

TempSet_& FG_Out(GRAPH::Node<LLVMIR::L_block*>* r) {
    return InOutTable[r].out;
}
TempSet_& FG_In(GRAPH::Node<LLVMIR::L_block*>* r) {
    return InOutTable[r].in;
}
TempSet_& FG_def(GRAPH::Node<LLVMIR::L_block*>* r) {
    return UseDefTable[r].def;
}
TempSet_& FG_use(GRAPH::Node<LLVMIR::L_block*>* r) {
    return UseDefTable[r].use;
}

static void Use_def(GRAPH::Node<LLVMIR::L_block*>* r,
                    GRAPH::Graph<LLVMIR::L_block*>& bg,
                    std::vector<Temp_temp*>& args) {
    //    args的作用尚不明确
    int dyeColor = 100;
    r->color = dyeColor;
    // 获取该block的Use & Def
    useDef curBlockUD = useDef();
    for (LLVMIR::L_stm* instr : r->info->instrs) {
        // 获取语句的def
        list<Temp_temp*> defTempList = get_def(instr);
        // 插入def
        for (auto it = defTempList.begin(); it != defTempList.end(); it++) {
            curBlockUD.def.insert(*it);
        }
        list<Temp_temp*> useTempList = get_use(instr);
        // 插入use，但不能是def的
        for (auto it = useTempList.begin(); it != useTempList.end(); it++) {
            if (!curBlockUD.def.count(*it)) {
                curBlockUD.use.insert(*it);
            }
        }
    }
    // 插入useDefTable
    UseDefTable[r] = curBlockUD;

    // 继续对未染色节点深搜
    for (auto succId : r->succs) {
        if (bg.mynodes[succId]->color != dyeColor) {
            Use_def(bg.mynodes[succId], bg, args);
        }
    }
}
static int gi = 0;
static bool LivenessIteration(GRAPH::Node<LLVMIR::L_block*>* r,
                              GRAPH::Graph<LLVMIR::L_block*>& bg) {
    // 染色
    int dyeColor = 299 + gi;
    r->color = dyeColor;
    // 针对自身进行iteration
    // 1. 新建元素的inOut，如果已经有了则不插入
    InOutTable.emplace(r, inOut());
    // 2. 更新in和out
    TempSet_& in = InOutTable[r].in;
    TempSet_& out = InOutTable[r].out;
    TempSet_& use = UseDefTable[r].use;
    TempSet_& def = UseDefTable[r].def;
    // 2.1 新in
    TempSet outdefDiff = TempSet_diff(&out, &def);
    TempSet newIn = TempSet_union(&use, outdefDiff);
    // 2.2 新out
    TempSet newOut = new TempSet_;

    for (int succId : r->succs) {
        GRAPH::Node<LLVMIR::L_block*>* nd = bg.mynodes[succId];
        newOut = TempSet_union(newOut, &InOutTable[nd].in);
    }
    // needMoreIter: 是否要继续下次迭代的变量。
    // 真假取决于后继们是否发生变化和自身是否发生变化
    bool needMoreIter = false;
    // 自身的in out是否发生变化
    needMoreIter = needMoreIter || !TempSet_eq(newIn, &in);
    needMoreIter = needMoreIter || !TempSet_eq(newOut, &out);
    // 不管怎么样都要更新自身
    in = *newIn;
    out = *newOut;
    // 3 深搜后继遍历其它元素。
    for (int succId : r->succs) {
        GRAPH::Node<LLVMIR::L_block*>* nd = bg.mynodes[succId];
        if (nd->color != dyeColor) {
            // 后继们是否发生变化
            needMoreIter = needMoreIter || LivenessIteration(nd, bg);
        }
    }

    return needMoreIter;
}

void PrintTemps(FILE* out, TempSet set) {
    for (auto x : *set) {
        printf("%d  ", x->num);
    }
}

void Show_Liveness(FILE* out, GRAPH::Graph<LLVMIR::L_block*>& bg) {
    fprintf(out, "\n\nNumber of iterations=%d\n\n", gi);
    for (auto block_node : bg.mynodes) {
        fprintf(out, "----------------------\n");
        printf("block %s \n", block_node.second->info->label->name.c_str());
        fprintf(out, "def=\n");
        PrintTemps(out, &FG_def(block_node.second));
        fprintf(out, "\n");
        fprintf(out, "use=\n");
        PrintTemps(out, &FG_use(block_node.second));
        fprintf(out, "\n");
        fprintf(out, "In=\n");
        PrintTemps(out, &FG_In(block_node.second));
        fprintf(out, "\n");
        fprintf(out, "Out=\n");
        PrintTemps(out, &FG_Out(block_node.second));
        fprintf(out, "\n");
    }
}
// 以block为单位
void Liveness(GRAPH::Node<LLVMIR::L_block*>* r,
              GRAPH::Graph<LLVMIR::L_block*>& bg,
              std::vector<Temp_temp*>& args) {
    init_INOUT();
    Use_def(r, bg, args);
    gi = 0;
    bool changed = true;
    while (changed) {
        changed = LivenessIteration(r, bg);
        // 一次DFS LivenessIteration之后，迭代次数gi++
        gi++;
    }
    // Show_Liveness(stdout, bg);
}
