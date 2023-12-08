#ifndef __SSA
#define __SSA

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "graph.hpp"
#include "llvm_ir.h"
#include "temp.h"

LLVMIR::L_prog* SSA(LLVMIR::L_prog* prog);
void combine_addr(LLVMIR::L_func* fun);
void mem2reg(LLVMIR::L_func* fun);
void Dominators(GRAPH::Graph<LLVMIR::L_block*>& bg);
void tree_Dominators(GRAPH::Graph<LLVMIR::L_block*>& bg);
void computeDF(GRAPH::Graph<LLVMIR::L_block*>& bg,
               GRAPH::Node<LLVMIR::L_block*>* r);
void Place_phi_fu(GRAPH::Graph<LLVMIR::L_block*>& bg, LLVMIR::L_func* fun);
void Rename(GRAPH::Graph<LLVMIR::L_block*>& bg);
void printf_domi();
void printf_D_tree();
void printf_DF();

// My Function
// 判断是否是标量
bool isScalar_Alloca(AS_operand* a);
// mem2reg中名字覆盖(temp_temp改至一样)
void cover(AS_operand* cur,
           std::unordered_map<AS_operand*, AS_operand*>& nameCover);

std::unordered_set<LLVMIR::L_block*> dom_union(
    std::unordered_set<LLVMIR::L_block*>& a,
    std::unordered_set<LLVMIR::L_block*>& b);

std::unordered_set<LLVMIR::L_block*> dom_intersection(
    std::unordered_set<LLVMIR::L_block*>& a,
    std::unordered_set<LLVMIR::L_block*>& b);

#endif