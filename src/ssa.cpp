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
std::vector<Temp_temp*> args;
static void init_table() {
    dominators.clear();
    tree_dominators.clear();
    DF_array.clear();
    revers_graph.clear();
    temp2ASoper.clear();
    args.clear();
}

LLVMIR::L_prog* SSA(LLVMIR::L_prog* prog) {
    int i = 0;
    for (auto& fun : prog->funcs) {
        printf("-------------ssa : block %d-------------\n", i++);
        init_table();
        args = fun->args;
        // AS_operand* 指向同一块 AS_operand 实例
        combine_addr(fun);
        mem2reg(fun);
        auto RA_bg = Create_bg(fun->blocks);
        SingleSourceGraph(RA_bg.mynodes[0], RA_bg, fun);
        // Show_graph(stdout,RA_bg);
        Liveness(RA_bg.mynodes[0], RA_bg, fun->args);
        Dominators(RA_bg);
        // printf_domi();
        tree_Dominators(RA_bg);
        // printf_D_tree();
        // // 默认0是入口block
        computeDF(RA_bg, RA_bg.mynodes[0]);
        // printf_DF();
        Place_phi_fu(RA_bg, fun);
        Rename(RA_bg);
        combine_addr(fun);
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
                    bool isFuncArg = false;
                    for (auto temp : fun->args){
                        if (temp == src->u.TEMP){
                            isFuncArg = true;
                        }
                    }
                    if (isFuncArg){
                        // 函数参数，需要move,但不需要cover
                        // 自动移动到下一条指令
                        it = instrs.erase(it);
                        // 添加move,
                        it = instrs.insert(it, L_Move(src, ptr));
                        // 移到下一条指令
                        it++;
                    }else {
                        // 是中间参数，不需要move
                        nameCover[src] = ptr;
                        // 自动移动到下一条指令
                         it = instrs.erase(it);
                    }
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
                        int iterTimes,
                        std::unordered_set<LLVMIR::L_block*>& allBlock) {
    int dyeColor = 99999 + iterTimes;
    // 为自身染色
    r->color = dyeColor;
    // 创建新Dom
    unordered_set<L_block*> newDom;
    newDom.emplace(r->info);
    // 得到前驱的交集
    unordered_set<L_block*> predIntersection;
    if (r->preds.size() != 0) {
        predIntersection = allBlock;
    }

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
    // 替换老Dom
    curDom = newDom;
    // 深搜遍历其余节点
    for (int succId : r->succs) {
        GRAPH::Node<LLVMIR::L_block*>* node = bg.mynodes[succId];
        if (node->color != dyeColor) {
            needMoreIter = DominatorIteration(node, bg, iterTimes, allBlock) ||
                           needMoreIter;
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
        changed = DominatorIteration(srcNode, bg, iterTimes, allBlock);
        iterTimes++;
    }
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
    // 生成支配树 BFS
    std::deque<int> q;
    q.push_back(0);
    std::set<int> flag;
    flag.insert(0);
    while (q.size() > 0) {
        GRAPH::Node<LLVMIR::L_block*>* node = bg.mynodes[q.front()];
        q.pop_front();
        LLVMIR::L_block* block = node->info;
        std::unordered_map<L_block*, unordered_set<L_block*>>::const_iterator
            iter = dominators.find(block);
        LLVMIR::L_block* only_idom = nullptr;
        int cnt = 0;
        for (LLVMIR::L_block* dom_block : iter->second) {
            if (dom_block == block)
                continue;  // 忽略掉自己
            bool isNotOthersDom = true;
            // 判断该Domer属不属于其它Domer的domee
            for (LLVMIR::L_block* _temp : iter->second) {
                if (_temp == dom_block)
                    continue;  // 忽略掉 dom_block 自己
                if (_temp == block)
                    continue;  // 忽略掉 block 自己
                std::unordered_map<L_block*,
                                   unordered_set<L_block*>>::const_iterator
                    _temp_iter = dominators.find(_temp);
                if (_temp_iter->second.count(dom_block)) {
                    // 如果出现在别的 支配节点的 支配set中
                    // 那么就是其它的支配节点的支配节点
                    isNotOthersDom = false;
                }
            }
            if (isNotOthersDom) {
                only_idom = dom_block;
                cnt++;
            } else {
                // 无事发生
            }
        }
        if (cnt > 1)
            assert(0);  // 看看是不是唯一的
        imm_Dominator id = imm_Dominator();
        id.pred = only_idom;
        id.succs = std::unordered_set<LLVMIR::L_block*>();
        if (only_idom) {
            // 如果存在 idom 那么要更新 idom 的 succs
            std::unordered_map<L_block*, imm_Dominator>::iterator iter =
                tree_dominators.find(only_idom);
            if (iter == tree_dominators.end())
                assert(0);
            iter->second.succs.insert(block);
        }
        tree_dominators.emplace(block, id);

        std::set<int> succs = node->succs;
        for (int succ : succs) {
            if (!flag.count(succ)) {
                // 未加入过的才能加入 BFS
                q.push_back(succ);
                flag.insert(succ);
            }
        }
    }
}

void computeDF(GRAPH::Graph<LLVMIR::L_block*>& bg,
               GRAPH::Node<LLVMIR::L_block*>* r) {
    // 创建reverse graph
    for (auto it = bg.mynodes.begin(); it != bg.mynodes.end(); it++) {
        revers_graph[it->second->info] = it->second;
    }

    // 伪代码的抄写
    std::unordered_set<L_block*> s;
    std::set<int> succs = r->succs;
    for (int succ : succs) {
        L_block* block_succ = bg.mynodes[succ]->info;
        if (tree_dominators[block_succ].pred != r->info) {
            s.insert(block_succ);
        }
    }
    for (L_block* block_child : tree_dominators[r->info].succs) {
        GRAPH::Node<LLVMIR::L_block*>* next = revers_graph[block_child];
        computeDF(bg, next);
        for (L_block* block_df_child : DF_array.find(block_child)->second) {
            // n 不是 w 的必经点 或者 n == w
            if (block_df_child == r->info ||
                !dominators.find(block_df_child)->second.count(r->info)) {
                s.insert(block_df_child);
            }
        }
    }
    DF_array.emplace(r->info, s);
}

// 只对标量做
void Place_phi_fu(GRAPH::Graph<LLVMIR::L_block*>& bg, L_func* fun) {

    // 只能转译伪代码
    std::unordered_map<Temp_temp*, std::unordered_set<GRAPH::Node<LLVMIR::L_block*>*>> defsites;
    // 记录以访问过的temp_temp
    std::set<Temp_temp*> temp_Visted; 
    std::deque<int> q;
    q.push_back(0);
    std::set<int> flag;
    flag.insert(0);
    while (q.size() > 0){
        GRAPH::Node<LLVMIR::L_block*>* node = bg.mynodes[q.front()];
        q.pop_front();
        std::unordered_set<Temp_temp*> def = FG_def(node);
        for (Temp_temp* tt: def){
            defsites.emplace(tt, std::unordered_set<GRAPH::Node<LLVMIR::L_block*>*>());
            temp_Visted.insert(tt);
            defsites.find(tt)->second.insert(node);
        }
        std::set<int> succs = node->succs;
        for (int succ: succs){
            if (!flag.count(succ)){
                // 未加入过的才能加入 BFS
                q.push_back(succ);
                flag.insert(succ);
            }
        }
    }

    std::unordered_map<LLVMIR::L_block*, std::unordered_set<Temp_temp*>> phi;
    for (Temp_temp* tt: temp_Visted){
        std::unordered_set<GRAPH::Node<LLVMIR::L_block*>*> W = defsites.find(tt)->second;
        while (W.size() > 0){
            std::unordered_set<GRAPH::Node<LLVMIR::L_block*>*>::iterator iter = W.begin();
            GRAPH::Node<LLVMIR::L_block*>* node = *iter;
            for (L_block* Y: DF_array.find(node->info)->second){
                phi.emplace(Y, std::unordered_set<Temp_temp*>());
                GRAPH::Node<LLVMIR::L_block*>* Y_node = revers_graph[Y];
                std::unordered_map<LLVMIR::L_block*, std::unordered_set<Temp_temp*>>::iterator phi_iter = phi.find(Y);
                if (!phi_iter->second.count(tt)){
                    AS_operand* aso = AS_Operand_Temp(tt);
                    std::vector<std::pair<AS_operand*,Temp_label*>> phis;
                    for (int Y_pred: Y_node->preds){
                        GRAPH::Node<LLVMIR::L_block*>* Y_pred_node = bg.mynodes[Y_pred];
                        Temp_label* label = Y_pred_node->info->label;
                        if (FG_In(Y_node).count(tt)){
                            // 只对在该节点live in的变量进行插phi
                            phis.push_back(std::pair<AS_operand*,Temp_label*>(aso, label));
                        }
                    }
                    if (phis.size() > 0){
                        std::list<L_stm*>::iterator Y_iter = Y->instrs.begin();
                        Y_iter ++;
                        Y->instrs.insert(Y_iter, LLVMIR::L_Phi(aso, phis));
                    }
                    phi_iter->second.insert(tt);
                    if (!FG_def(Y_node).count(tt)){
                        W.insert(Y_node);
                    }
                }
            }
            W.erase(iter);
        }
    }
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

AS_operand* Rename_temp_def(std::unordered_map<Temp_temp*, int>& count, std::unordered_map<Temp_temp*, std::stack<Temp_temp*>>& stack, AS_operand* aso, std::vector<Temp_temp*>& Rename_def){
    if (aso == nullptr) return aso;
    if (aso->kind != OperandKind::TEMP){
        return aso;
    }
    Temp_temp* tt = aso->u.TEMP;
    // 初始化
    count.emplace(tt, 0);
    stack.emplace(tt, std::stack<Temp_temp*>());

    if (tt->type != TempType::INT_TEMP){
        count.find(tt)->second ++;
        stack.find(tt)->second.push(tt);
        return AS_Operand_Temp(tt);
    }

    // 只处理标量
    Temp_temp *new_tt = Temp_newtemp_int();
    count.find(tt)->second ++;
    stack.find(tt)->second.push(new_tt);
    Rename_def.push_back(tt);

    AS_operand* new_aso = AS_Operand_Temp(new_tt);
    return new_aso;
}

AS_operand* Rename_temp_use(std::unordered_map<Temp_temp*, int>& count, std::unordered_map<Temp_temp*, std::stack<Temp_temp*>>& stack, AS_operand* aso){
    if (aso == nullptr) return aso;
    if (aso->kind != OperandKind::TEMP){
        return aso;
    }
    Temp_temp* tt = aso->u.TEMP;
    if (count.find(tt) == count.end()){
        // 如果不是函数
        bool isArg = false;
        for (Temp_temp* arg: args){
            if (arg == tt){
                isArg = true;
                break;
            }
        }
        // use 在 def 之前 而且也不是参数
        if (isArg) return aso;
        assert(0);
    }
    
    return AS_Operand_Temp(stack.find(tt)->second.top());
}

void Rename(std::unordered_map<Temp_temp*, int>& count, std::unordered_map<Temp_temp*, std::stack<Temp_temp*>>& stack, GRAPH::Graph<LLVMIR::L_block*>& bg, GRAPH::Node<LLVMIR::L_block*>*& node){
    // Rename 本质是一个 DFS
    std::list<L_stm*>::iterator iter = node->info->instrs.begin();
    // 记录 def 之后要弹出的
    std::vector<Temp_temp*> Rename_def;
    while (iter!=node->info->instrs.end()){
        L_stm* S = *iter;
        switch (S->type)
        {
        case L_StmKind::T_ALLOCA:{
            AS_operand *new_aso = Rename_temp_def(count, stack, S->u.ALLOCA->dst, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Alloca(new_aso));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;
        
        case L_StmKind::T_BINOP:{
            AS_operand *aso_left = S->u.BINOP->dst, *aso_right_1 = S->u.BINOP->left, *aso_right_2 = S->u.BINOP->right;
            AS_operand *new_aso_right_1 = Rename_temp_use(count, stack, aso_right_1);
            AS_operand *new_aso_right_2 = Rename_temp_use(count, stack, aso_right_2);
            AS_operand *new_aso_left = Rename_temp_def(count, stack, aso_left, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Binop(S->u.BINOP->op, new_aso_right_1, new_aso_right_2, new_aso_left));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_CALL:{
            AS_operand *aso_left = S->u.CALL->res;
            std::vector<AS_operand*> args = S->u.CALL->args;
            std::vector<AS_operand*> new_args;
            for (AS_operand* arg: args){
                new_args.push_back(Rename_temp_use(count, stack, arg));
            }
            AS_operand *new_aso_left = Rename_temp_def(count, stack, aso_left, Rename_def);
            
            node->info->instrs.insert(iter, LLVMIR::L_Call(S->u.CALL->fun, new_aso_left, new_args));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_CJUMP:{
            AS_operand *aso = S->u.CJUMP->dst;
            AS_operand *new_aso = Rename_temp_use(count, stack, aso);
            node->info->instrs.insert(iter, LLVMIR::L_Cjump(new_aso, S->u.CJUMP->true_label, S->u.CJUMP->false_label));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        
        case L_StmKind::T_CMP:{
            AS_operand *aso_left = S->u.CMP->dst, *aso_right_1 = S->u.CMP->left, *aso_right_2 = S->u.CMP->right;
            AS_operand *new_aso_right_1 = Rename_temp_use(count, stack, aso_right_1);
            AS_operand *new_aso_right_2 = Rename_temp_use(count, stack, aso_right_2);
            AS_operand *new_aso_left = Rename_temp_def(count, stack, aso_left, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Cmp(S->u.CMP->op, new_aso_right_1, new_aso_right_2, new_aso_left));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_GEP:{
            AS_operand* base_aso = Rename_temp_use(count, stack, S->u.GEP->base_ptr);
            AS_operand* index_aso = Rename_temp_use(count, stack, S->u.GEP->index);
            AS_operand* ptr_aso = Rename_temp_def(count, stack, S->u.GEP->new_ptr, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Gep(ptr_aso, base_aso, index_aso));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_JUMP:{
            iter ++;
            continue;
        }
            break;

        case L_StmKind::T_LABEL:{
            iter ++;
            continue;
        }
            break;

        case L_StmKind::T_LOAD:{
            AS_operand *ptr = Rename_temp_use(count, stack, S->u.LOAD->ptr);
            AS_operand *dst = Rename_temp_def(count, stack, S->u.LOAD->dst, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Load(dst, ptr));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_MOVE:{
            AS_operand *src = Rename_temp_use(count, stack, S->u.MOVE->src);
            AS_operand *dst = Rename_temp_def(count, stack, S->u.MOVE->dst, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Move(src, dst));
            iter = node->info->instrs.erase(iter);
            continue;
        }

        case L_StmKind::T_NULL:{
            iter ++;
            continue;
        }
            break;

        case L_StmKind::T_PHI:{
            // 这里暂时不处理 phi 语句 ? Phi 的 def 也要加入进来！ use 不用
            AS_operand *dst = Rename_temp_def(count, stack, S->u.PHI->dst, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Phi(dst, S->u.PHI->phis));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_RETURN:{
            AS_operand *ret = Rename_temp_use(count, stack, S->u.RET->ret);
            node->info->instrs.insert(iter, LLVMIR::L_Ret(ret));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_STORE:{
            AS_operand *src = Rename_temp_use(count, stack, S->u.STORE->src);
            AS_operand *ptr = Rename_temp_def(count, stack, S->u.STORE->ptr, Rename_def);
            node->info->instrs.insert(iter, LLVMIR::L_Store(src, ptr));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        case L_StmKind::T_VOID_CALL:{
            std::vector<AS_operand*> args = S->u.VOID_CALL->args;
            std::vector<AS_operand*> new_args;
            for (AS_operand* arg: args){
                new_args.push_back(Rename_temp_use(count, stack, arg));
            }
            node->info->instrs.insert(iter, LLVMIR::L_Voidcall(S->u.CALL->fun, new_args));
            iter = node->info->instrs.erase(iter);
            continue;
        }
            break;

        default:
            break;
        }
    }

    std::set<int> succs = node->succs;
    for (int succ: succs){
        GRAPH::Node<LLVMIR::L_block*>* node_succ = bg.mynodes[succ];
        for (L_stm* stm: node_succ->info->instrs){
            if (stm->type == L_StmKind::T_PHI){
                for (int i=0;i<stm->u.PHI->phis.size();i++){
                    if (stm->u.PHI->phis[i].second == node->info->label){
                        AS_operand *temp = Rename_temp_use(count, stack, stm->u.PHI->phis[i].first);
                        stm->u.PHI->phis[i].first = temp;
                        break;
                    }
                }
            }
        }
    }

    // FIXME: 你他妈的 遍历的是支配树？
    node->color = -1;
    imm_Dominator id = tree_dominators.find(node->info)->second;
    for (L_block* succ: id.succs){
        if (revers_graph[succ]->color == -1) assert(0);
        Rename(count, stack, bg, revers_graph[succ]);
    }

    // 弹出之前压入栈的一切
    for (Temp_temp* tt: Rename_def){
        count.find(tt)->second --;
        stack.find(tt)->second.pop();
    }
}

void Rename(GRAPH::Graph<LLVMIR::L_block*>& bg) {
    // redefinition phi 所有的重命名都在这里
    std::unordered_map<Temp_temp*, int> count;
    std::unordered_map<Temp_temp*, std::stack<Temp_temp*>> stack;
    Rename(count, stack, bg, bg.mynodes[0]);
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
