#include "ast2llvm.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include <list>

using namespace std;
using namespace LLVMIR;

static unordered_map<string,FuncType> funcReturnMap;  // 函数名字-> returnType
static unordered_map<string,StructInfo> structInfoMap; // sturct =>  struct的属性
static unordered_map<string,Name_name*> globalVarMap; // 全局变量名->变量的寄存器信息（名字）
static unordered_map<string,Temp_temp*> localVarMap; // 局部变量名
static list<L_stm*> emit_irs;

static unordered_map<string, Temp_temp*> funcParamMap; // 扫完一张函数表就清空
// 注意这个参数在void和非void call里都有使用
static bool isInFunc = false; // 标记是否在函数当中，用于区分rightVal是否需要load
LLVMIR::L_prog* ast2llvm(aA_program p)
{
    auto defs = ast2llvmProg_first(p);
    auto funcs = ast2llvmProg_second(p);
    vector<L_func*> funcs_block;
    for(const auto &f : funcs)
    {
        funcs_block.push_back(ast2llvmFuncBlock(f));
    }
    // 每个块alloca挪到最前
    for(auto &f : funcs_block)
    {
        ast2llvm_moveAlloca(f);
    }
    return new L_prog(defs,funcs_block);
}

int ast2llvmRightVal_first(aA_rightVal r )
{
    if(r == nullptr)
    {
        return 0;
    }
    switch (r->kind)
    {
    case A_arithExprValKind:
    {
        return ast2llvmArithExpr_first(r->u.arithExpr);
        break;
    }
    case A_boolExprValKind:
    {
        return ast2llvmBoolExpr_first(r->u.boolExpr);
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmBoolExpr_first(aA_boolExpr b)
{
    switch (b->kind)
    {
    case A_boolBiOpExprKind:
    {
        return ast2llvmBoolBiOpExpr_first(b->u.boolBiOpExpr);
        break;
    }
    case A_boolUnitKind:
    {
        return ast2llvmBoolUnit_first(b->u.boolUnit);
        break;
    }
    default:
         break;
    }
    return 0;
}

int ast2llvmBoolBiOpExpr_first(aA_boolBiOpExpr b)
{
    int l = ast2llvmBoolExpr_first(b->left);
    int r = ast2llvmBoolExpr_first(b->right);
    if(b->op == A_and)
    {
        return l && r;
    }
    else
    {
        return l || r;
    }
}

int ast2llvmBoolUOpExpr_first(aA_boolUOpExpr b)
{
    if(b->op == A_not)
    {
        return !ast2llvmBoolUnit_first(b->cond);
    }
    return 0;
}

int ast2llvmBoolUnit_first(aA_boolUnit b)
{
    switch (b->kind)
    {
    case A_comOpExprKind:
    {
        return ast2llvmComOpExpr_first(b->u.comExpr);
        break;
    }
    case A_boolExprKind:
    {
        return ast2llvmBoolExpr_first(b->u.boolExpr);
        break;
    }
    case A_boolUOpExprKind:
    {
        return ast2llvmBoolUOpExpr_first(b->u.boolUOpExpr);
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmComOpExpr_first(aA_comExpr c)
{
    auto l = ast2llvmExprUnit_first(c->left);
    auto r = ast2llvmExprUnit_first(c->right);
    switch (c->op)
    {
    case A_lt:
    {
        return l < r;
        break;
    }
    case A_le:
    {
        return l <= r;
        break;
    }
    case A_gt:
    {
        return l > r;
        break;
    }
    case A_ge:
    {
        return l >= r;
        break;
    }
    case A_eq:
    {
        return l == r;
        break;
    }
    case A_ne:
    {
        return l != r;
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmArithBiOpExpr_first(aA_arithBiOpExpr a)
{
    auto l= ast2llvmArithExpr_first(a->left);
    auto r = ast2llvmArithExpr_first(a->right);
    switch (a->op)
    {
    case A_add:
    {
        return l + r;
        break;
    }
    case A_sub:
    {
        return l - r;
        break;
    }
    case A_mul:
    {
        return l * r;
        break;
    }
    case A_div:
    {
        return l / r;
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmArithUExpr_first(aA_arithUExpr a)
{
    if(a->op == A_neg)
    {
        return -ast2llvmExprUnit_first(a->expr);
    }
    return 0;
}

int ast2llvmArithExpr_first(aA_arithExpr a)
{
    switch (a->kind)
    {
    case A_arithBiOpExprKind:
    {
        return ast2llvmArithBiOpExpr_first(a->u.arithBiOpExpr);
        break;
    }
    case A_exprUnitKind:
    {
        return ast2llvmExprUnit_first(a->u.exprUnit);
        break;
    }
    default:
        assert(0);
        break;
    }
    return 0;
}

int ast2llvmExprUnit_first(aA_exprUnit e)
{
    if(e->kind == A_numExprKind)
    {
        return e->u.num;
    }
    else if(e->kind == A_arithExprKind)
    {
        return ast2llvmArithExpr_first(e->u.arithExpr);
    }
    else if(e->kind == A_arithUExprKind)
    {
        return ast2llvmArithUExpr_first(e->u.arithUExpr);
    }
    else
    {
        assert(0);
    }
    return 0;
}


std::vector<LLVMIR::L_def*> ast2llvmProg_first(aA_program p)
{
    vector<L_def*> defs; // 存放全局变量、结构体定义、函数声明
    defs.push_back(L_Funcdecl("getch",vector<TempDef>(),FuncType(ReturnType::INT_TYPE)));
    defs.push_back(L_Funcdecl("getint",vector<TempDef>(),FuncType(ReturnType::INT_TYPE)));
    defs.push_back(L_Funcdecl("putch",vector<TempDef>{TempDef(TempType::INT_TEMP)},FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("putint",vector<TempDef>{TempDef(TempType::INT_TEMP)},FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("putarray",vector<TempDef>{TempDef(TempType::INT_TEMP),TempDef(TempType::INT_PTR,-1)},FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("_sysy_starttime",vector<TempDef>{TempDef(TempType::INT_TEMP)},FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("_sysy_stoptime",vector<TempDef>{TempDef(TempType::INT_TEMP)},FuncType(ReturnType::VOID_TYPE)));
    // 添加库函数的returnType
    funcReturnMap["getch"] = FuncType(ReturnType::INT_TYPE);
    funcReturnMap["getint"] = FuncType(ReturnType::INT_TYPE);
    funcReturnMap["putch"] = FuncType(ReturnType::VOID_TYPE);
    funcReturnMap["putint"] = FuncType(ReturnType::VOID_TYPE);
    funcReturnMap["putarray"] = FuncType(ReturnType::VOID_TYPE);
    funcReturnMap["_sysy_starttime"] = FuncType(ReturnType::VOID_TYPE);
    funcReturnMap["_sysy_stoptime"] = FuncType(ReturnType::VOID_TYPE);
    //
    
    for(const auto &v : p->programElements)
    {
        switch (v->kind)
        {
        case A_programNullStmtKind:
        {
            break;
        }
        // 全局变量的定义与声明
        case A_programVarDeclStmtKind:
        {
            if(v->u.varDeclStmt->kind == A_varDeclKind)
            {
                if(v->u.varDeclStmt->u.varDecl->kind == A_varDeclScalarKind)
                {
                    if(v->u.varDeclStmt->u.varDecl->u.declScalar->type->type == A_structTypeKind)
                    {
                        // 放入全局变量表，对应Temp的名字是自己的id，也就是变量的名字
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declScalar->id,
                            Name_newname_struct(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declScalar->id),*v->u.varDeclStmt->u.varDecl->u.declScalar->type->u.structType));
                        // 增加一个全局寄存器定义（名字是变量的id），现在可以通过id找到对应寄存器
                        // 寄存器到时候在IR时创建
                        TempDef def(TempType::STRUCT_TEMP,0,*v->u.varDeclStmt->u.varDecl->u.declScalar->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declScalar->id,def,vector<int>()));
                    }
                    else
                    {
                        // int变量声明，加入全局变量表
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declScalar->id,
                            Name_newname_int(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declScalar->id)));
                        TempDef def(TempType::INT_TEMP,0);
                        // 增加该定义，之后打印
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declScalar->id,def,vector<int>()));
                    }
                }
                else if(v->u.varDeclStmt->u.varDecl->kind == A_varDeclArrayKind)
                {
                    if(v->u.varDeclStmt->u.varDecl->u.declArray->type->type == A_structTypeKind)
                    {
                        // struct数组，加入全局变量
                        // 寄存器名字是变量名字
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declArray->id,
                            Name_newname_struct_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declArray->id),v->u.varDeclStmt->u.varDecl->u.declArray->len,*v->u.varDeclStmt->u.varDecl->u.declArray->type->u.structType));
                        // 添加数组指针型寄存器定义
                        TempDef def(TempType::STRUCT_PTR,v->u.varDeclStmt->u.varDecl->u.declArray->len,*v->u.varDeclStmt->u.varDecl->u.declArray->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declArray->id,def,vector<int>()));
                    }
                    else
                    {
                        // int数组
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declArray->id,
                            Name_newname_int_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declArray->id),v->u.varDeclStmt->u.varDecl->u.declArray->len));
                        TempDef def(TempType::INT_PTR,v->u.varDeclStmt->u.varDecl->u.declArray->len);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declArray->id,def,vector<int>()));
                    }
                }
                else
                {
                    assert(0);
                }
            }
            else if(v->u.varDeclStmt->kind == A_varDefKind)
            {
                if(v->u.varDeclStmt->u.varDef->kind == A_varDefScalarKind)
                {
                    if(v->u.varDeclStmt->u.varDef->u.defScalar->type->type == A_structTypeKind)
                    {
                        // 仍旧是创建全局变量定义，寄存器名字是它的id
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defScalar->id,
                            Name_newname_struct(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defScalar->id),*v->u.varDeclStmt->u.varDef->u.defScalar->type->u.structType));
                        // 寄存器定义，名字是id，
                        TempDef def(TempType::STRUCT_TEMP,0,*v->u.varDeclStmt->u.varDef->u.defScalar->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defScalar->id,def,vector<int>()));
                    }
                    else
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defScalar->id,
                            Name_newname_int(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defScalar->id)));
                        TempDef def(TempType::INT_TEMP,0);
                        vector<int> init;
                        // int值放进去寄存器声明中
                        init.push_back(ast2llvmRightVal_first(v->u.varDeclStmt->u.varDef->u.defScalar->val));
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defScalar->id,def,init));
                    }
                }
                else if(v->u.varDeclStmt->u.varDef->kind == A_varDefArrayKind)
                {
                    if(v->u.varDeclStmt->u.varDef->u.defArray->type->type == A_structTypeKind)
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defArray->id,
                            Name_newname_struct_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defArray->id),v->u.varDeclStmt->u.varDef->u.defArray->len,*v->u.varDeclStmt->u.varDef->u.defArray->type->u.structType));
                        TempDef def(TempType::STRUCT_PTR,v->u.varDeclStmt->u.varDef->u.defArray->len,*v->u.varDeclStmt->u.varDef->u.defArray->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defArray->id,def,vector<int>()));
                    }
                    else
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defArray->id,
                            Name_newname_int_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defArray->id),v->u.varDeclStmt->u.varDef->u.defArray->len));
                        TempDef def(TempType::INT_PTR,v->u.varDeclStmt->u.varDef->u.defArray->len);
                        vector<int> init;
                        for(auto &el : v->u.varDeclStmt->u.varDef->u.defArray->vals)
                        {
                            init.push_back(ast2llvmRightVal_first(el));
                        }
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defArray->id,def,init));
                    }
                }
                else
                {
                    assert(0);
                }
            }
            else
            {
                assert(0);
            }
            break;
        }
        case A_programStructDefKind:
        {
            StructInfo si;
            int off = 0;
            vector<TempDef> members;
            for(const auto &decl : v->u.structDef->varDecls)
            {
                if(decl->kind == A_varDeclScalarKind)
                {
                    if(decl->u.declScalar->type->type == A_structTypeKind)
                    {
                        // Struct寄存器
                        TempDef def(TempType::STRUCT_TEMP,0,*decl->u.declScalar->type->u.structType);
                        // 注入structInfo,info内部是一个map，属性id->memberinfo
                        // info记录偏移量和寄存器
                        MemberInfo info(off++,def);
                        si.memberinfos.emplace(*decl->u.declScalar->id,info);
                        members.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_TEMP,0);
                        MemberInfo info(off++,def);
                        si.memberinfos.emplace(*decl->u.declScalar->id,info);
                        members.push_back(def);
                    }
                }
                else if(decl->kind == A_varDeclArrayKind)
                {
                    if(decl->u.declArray->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_PTR,decl->u.declArray->len,*decl->u.declArray->type->u.structType);
                        MemberInfo info(off++,def);
                        si.memberinfos.emplace(*decl->u.declArray->id,info);
                        members.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_PTR,decl->u.declArray->len);
                        MemberInfo info(off++,def);
                        si.memberinfos.emplace(*decl->u.declArray->id,info);
                        members.push_back(def);
                    }
                }
                else
                {
                    assert(0);
                }
            }
            // 添加到structInfoMap，struct name -> 其属性列表
            structInfoMap.emplace(*v->u.structDef->id,std::move(si));
            // 增加
            defs.push_back(L_Structdef(*v->u.structDef->id,members));
            break;
        }
        case A_programFnDeclStmtKind:
        {
            FuncType type;
            if(v->u.fnDeclStmt->fnDecl->type == nullptr)
            {
                type.type = ReturnType::VOID_TYPE;
            }
            if(v->u.fnDeclStmt->fnDecl->type->type == A_nativeTypeKind)
            {
                type.type = ReturnType::INT_TYPE;
            }
            else if(v->u.fnDeclStmt->fnDecl->type->type == A_structTypeKind)
            {
                type.type = ReturnType::STRUCT_TYPE;
                type.structname = *v->u.fnDeclStmt->fnDecl->type->u.structType;
            }
            else
            {
                assert(0);
            }
            // 添加函数返回值类型
            if(funcReturnMap.find(*v->u.fnDeclStmt->fnDecl->id) == funcReturnMap.end())
                funcReturnMap.emplace(*v->u.fnDeclStmt->fnDecl->id,std::move(type));
            vector<TempDef> args;
            for(const auto & decl : v->u.fnDeclStmt->fnDecl->paramDecl->varDecls)
            {
                if(decl->kind == A_varDeclScalarKind)
                {
                    if(decl->u.declScalar->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_PTR,0,*decl->u.declScalar->type->u.structType);
                        args.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_TEMP,0);
                        args.push_back(def);
                    }
                }
                else if(decl->kind == A_varDeclArrayKind)
                {
                    if(decl->u.declArray->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_PTR,-1,*decl->u.declArray->type->u.structType);
                        args.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_PTR,-1);
                        args.push_back(def);
                    }
                }
                else
                {
                    assert(0);
                }
            }
            defs.push_back(L_Funcdecl(*v->u.fnDeclStmt->fnDecl->id,args,type));
            break;
        }
        case A_programFnDefKind:
        {
            if(funcReturnMap.find(*v->u.fnDef->fnDecl->id) == funcReturnMap.end())
            {
                FuncType type;
                if(v->u.fnDef->fnDecl->type == nullptr)
                {
                    type.type = ReturnType::VOID_TYPE;
                }
                else if(v->u.fnDef->fnDecl->type->type == A_nativeTypeKind)
                {
                    type.type = ReturnType::INT_TYPE;
                }
                else if(v->u.fnDef->fnDecl->type->type == A_structTypeKind)
                {
                    type.type = ReturnType::STRUCT_TYPE;
                    type.structname = *v->u.fnDef->fnDecl->type->u.structType;
                }
                else
                {
                    assert(0);
                }
                funcReturnMap.emplace(*v->u.fnDef->fnDecl->id,std::move(type));
            }
            break;
        }
        default:
            assert(0);
            break;
        }
    }
    return defs;
}

std::vector<Func_local*> ast2llvmProg_second(aA_program p)
{   
    vector<Func_local*> func_local;
    // func_local 包含f_name ,ret_type,参数寄存器，以及各行的ir
    for(const auto &v : p->programElements)
    {
        if (v->kind == A_programElementType::A_programFnDefKind){
            func_local.push_back(ast2llvmFunc(v->u.fnDef));
        }
    }
    return func_local;
}

Func_local* ast2llvmFunc(aA_fnDef f)
{
    // 函数名字
    string name = *(f->fnDecl->id);
    // 返回类型
    FuncType ret;
    if(f->fnDecl->type == nullptr)
    {
        ret.type = ReturnType::VOID_TYPE;
    }
    else if(f->fnDecl->type->type == A_nativeTypeKind)
    {
        ret.type = ReturnType::INT_TYPE;
    }
    else if(f->fnDecl->type->type == A_structTypeKind)
    {
        ret.type = ReturnType::STRUCT_TYPE;
        ret.structname = *f->fnDecl->type->u.structType;
    }
    else
    {
        assert(0);
    }
    // 添加函数的总起label
    emit_irs.emplace_back(L_Label(Temp_newlabel()));

    vector<Temp_temp*> args;
    for(const auto & decl : f->fnDecl->paramDecl->varDecls){
        Temp_temp* arg;
        if (decl->kind == A_varDeclType::A_varDeclScalarKind){
            aA_varDeclScalar vds= decl->u.declScalar;
            if (vds->type->type == A_structTypeKind){
                arg = Temp_newtemp_struct_ptr(0,*vds->type->u.structType);
                // 添加函数参数类型
                funcParamMap[*vds->id]=arg;
            }else{
                // 针对int类型的参数需要进行alloca
                arg = Temp_newtemp_int(); // 这个寄存器对应的是参数寄存器
                // 现在写alloca
                Temp_temp* use_in_func_arg = Temp_newtemp_int_ptr(0); // 这个寄存器是函数使用的寄存器
                funcParamMap[*vds->id] = use_in_func_arg;
                emit_irs.emplace_back(L_Alloca(AS_Operand_Temp(use_in_func_arg)));
                emit_irs.emplace_back(L_Store(AS_Operand_Temp(arg),AS_Operand_Temp(use_in_func_arg)));
            }
        }else if (decl->kind == A_varDeclType::A_varDeclArrayKind){
            aA_varDeclArray vda = decl->u.declArray;
            if (vda->type->type == A_structTypeKind){
                arg = Temp_newtemp_struct_ptr(-1, *vda->type->u.structType);
                funcParamMap[*vda->id]=arg;
            }else{
                arg = Temp_newtemp_int_ptr(-1);
                funcParamMap[*vda->id]=arg;
            }
        }
        args.push_back(arg);
        // 将函数参数记录为局部变量
    }
 
    // 遍历函数体的block
    int sentenceNum = 1;
    for (const auto& stmt : f->stmts){
        ast2llvmBlock(stmt);
        printf("完成 %d 行函数内定义代码\n",sentenceNum++);
    }
    // 末尾return 添加
    L_stm* last = emit_irs.back();
    if (last->type !=L_StmKind::T_RETURN ){
        if (ret.type == ReturnType::VOID_TYPE){
            emit_irs.push_back(L_Ret(nullptr));
        }else{
            emit_irs.push_back(L_Ret(AS_Operand_Const(0)));
        }
    }
    printf("装载函数进func_local\n");
    Func_local* f_local = new Func_local(name,ret,args,emit_irs);
    printf("%s emit irs len %zu\n",name.c_str(),emit_irs.size());
    emit_irs.clear();
    // 清空函数参数表
    funcParamMap.clear();
    // 清空临时参数列表（因为emplace）
    localVarMap.clear();
  
    return f_local;
}

void ast2llvmBlock(aA_codeBlockStmt b,Temp_label *con_label,Temp_label *bre_label)
{
    if (b->kind == A_codeBlockStmtType::A_varDeclStmtKind){
        aA_varDeclStmt vd = b->u.varDeclStmt;
        if (vd->kind == A_varDeclStmtType::A_varDeclKind){
            aA_varDecl vdecl = vd->u.varDecl;
            if (vdecl->kind ==  A_varDeclType::A_varDeclScalarKind){
                aA_varDeclScalar vds = vdecl->u.declScalar;
                Temp_temp* t;
                if (vds->type->type == A_dataType::A_structTypeKind){
                    t = Temp_newtemp_struct_ptr(0,*vds->type->u.structType);
                }else{
                    t = Temp_newtemp_int_ptr(0);
                }
                // 添加到local表 && 添加ir
                localVarMap[*vds->id] = t;
                emit_irs.push_back(L_Alloca(AS_Operand_Temp(t)));
            }else if (vdecl->kind ==  A_varDeclType::A_varDeclArrayKind){
                aA_varDeclArray vda = vdecl->u.declArray;
                Temp_temp* t;
                if (vda->type->type == A_dataType::A_structTypeKind){
                    printf("%s数组[%d]\n",vda->type->u.structType->c_str(),vda->len);
                    t = Temp_newtemp_struct_ptr(vda->len,*vda->type->u.structType);
                }else{
                    t = Temp_newtemp_int_ptr(vda->len);
                }
                // 添加到local表 && 添加ir
                localVarMap[*vda->id]=t;
                emit_irs.push_back(L_Alloca(AS_Operand_Temp(t))) ;
            }
        }else if (vd->kind == A_varDeclStmtType::A_varDefKind){
            aA_varDef vdef = vd->u.varDef;
            if (vdef->kind == A_varDefType::A_varDefScalarKind){
                aA_varDefScalar vds = vdef->u.defScalar;
                Temp_temp* t;
                if (vds->type->type == A_dataType::A_structTypeKind){
                    t = Temp_newtemp_struct_ptr(0,*vds->type->u.structType);
                }else{
                    t = Temp_newtemp_int_ptr(0);
                }
                // 添加到local表
                localVarMap[*vds->id]= t;
                // 添加ir
                AS_operand* dst = AS_Operand_Temp(t);
                 emit_irs.emplace_back(L_Alloca(dst));
                AS_operand* src = ast2llvmRightVal(vds->val);
                emit_irs.emplace_back(L_Store(src,dst));
            }else if (vdef->kind == A_varDefType::A_varDefArrayKind){
                aA_varDefArray vda = vdef->u.defArray;
                Temp_temp* t;
                if (vda->type->type == A_dataType::A_structTypeKind){
                    t = Temp_newtemp_struct_ptr(vda->len,*vda->type->u.structType);
                    // 不会出现结构体数组直接赋值，应该是没有结构体数组的
                    assert(0);
                }else{
                    // 数组基指针寄存器
                    t = Temp_newtemp_int_ptr(vda->len);
                    // 添加到local表
                    localVarMap[*vda->id]= t;
                    // 添加ir
                    AS_operand* dst = AS_Operand_Temp(t);
                    emit_irs.emplace_back(L_Alloca(dst));
                    int rval_index = 0;
                    for (const aA_rightVal rval : vda->vals){
                        // 数组元素寄存器
                        AS_operand* eleOperand = AS_Operand_Temp(Temp_newtemp_int());
                        AS_operand* valOperand = ast2llvmRightVal(rval);
                        AS_operand* indexOperand = AS_Operand_Const(rval_index++);
                        // 先GEP
                        emit_irs.emplace_back(L_Gep(eleOperand,dst,indexOperand));
                        // 再store
                        emit_irs.emplace_back(L_Store(valOperand, eleOperand));
                    }
                }
            }
        }
    }
    else if (b->kind == A_codeBlockStmtType::A_assignStmtKind){
        aA_assignStmt as = b->u.assignStmt;
        AS_operand* right = ast2llvmRightVal(as->rightVal);
        AS_operand* left = ast2llvmLeftVal(as->leftVal);
        emit_irs.emplace_back(L_Store(right,left));
    }
    else if (b->kind == A_codeBlockStmtType::A_callStmtKind){
        aA_fnCall fnCall = b->u.callStmt->fnCall; 
        vector<AS_operand*> args;
        isInFunc = true;
        for (auto rval : fnCall->vals){
            args.push_back(ast2llvmRightVal(rval));
        }
        isInFunc =false;
        emit_irs.emplace_back(L_Voidcall(*fnCall->fn,args));
    }
    else if (b->kind == A_codeBlockStmtType::A_ifStmtKind){
        aA_ifStmt ifstmt = b->u.ifStmt;
        // 有if和else
        Temp_label* trueLabel  = Temp_newlabel();
        Temp_label* falseLabel  = Temp_newlabel();
        Temp_label* mergeLabel  = Temp_newlabel();
        // 1. 先对boolExpr进行翻译
        AS_operand* cmp =  ast2llvmBoolExpr(ifstmt->boolExpr,trueLabel,falseLabel);
        if (cmp) emit_irs.emplace_back(L_Cjump(cmp,trueLabel,falseLabel)); // 针对一元boolExpr添加Cjump，如果是二元，内部已经有Cjump
        // 2. 创建true\false\merge的内容
        emit_irs.emplace_back(L_Label(trueLabel));
        for (const auto& codeblock : ifstmt->ifStmts){
            ast2llvmBlock(codeblock,con_label,bre_label);
        }
        emit_irs.emplace_back(L_Jump(mergeLabel));
        emit_irs.emplace_back(L_Label(falseLabel));
        for (const auto& codeblock : ifstmt->elseStmts){
            ast2llvmBlock(codeblock,con_label,bre_label);
        }
        emit_irs.emplace_back(L_Jump(mergeLabel));
        emit_irs.emplace_back(L_Label(mergeLabel));
    }
    else if (b->kind == A_codeBlockStmtType::A_whileStmtKind){
        aA_whileStmt wstmt = b->u.whileStmt;
        // 分成3块，loopHeader,loopBody,loopExit
        Temp_label* loopHeader = Temp_newlabel();
        Temp_label* loopBody = Temp_newlabel();
        Temp_label* loopExit = Temp_newlabel();
        // 1. 进入header
        emit_irs.emplace_back(L_Jump(loopHeader));
        // 2. 在header翻译boolExpr
        emit_irs.emplace_back(L_Label(loopHeader));
        AS_operand* cmp =  ast2llvmBoolExpr(wstmt->boolExpr,loopBody,loopExit);
        if (cmp) emit_irs.emplace_back(L_Cjump(cmp,loopBody,loopExit)); // 仿照ifstmt
        // 3. 在body里干翻译循环体内的代码
        emit_irs.emplace_back(L_Label(loopBody));
        for (const auto& codeBlock : wstmt->whileStmts){
            ast2llvmBlock(codeBlock,loopHeader,loopExit);
        }
        emit_irs.emplace_back(L_Jump(loopHeader));
        // 4. while之外代码的lable
        emit_irs.emplace_back(L_Label(loopExit));
    }
    else if (b->kind == A_codeBlockStmtType::A_returnStmtKind){
        aA_returnStmt retStmt = b->u.returnStmt;
        emit_irs.emplace_back(L_Ret(ast2llvmRightVal(retStmt->retVal)));
    }
    else if (b->kind == A_codeBlockStmtType::A_continueStmtKind){
        if (!con_label){
            // 出现空指针问题
            assert(0);
        }
        emit_irs.emplace_back(L_Jump(con_label));
    }
    else if (b->kind == A_codeBlockStmtType::A_breakStmtKind){
        if (!bre_label){
            // 出现空指针问题
            assert(0);
        }
        emit_irs.emplace_back(L_Jump(bre_label));
    }
}

AS_operand* ast2llvmRightVal(aA_rightVal r)
{  
    // 针对void call
    if (!r ){
        return nullptr;
    }
    if (r->kind == A_rightValType::A_arithExprValKind){
        return ast2llvmArithExpr(r->u.arithExpr);
    }else if (r->kind == A_rightValType::A_boolExprValKind){
        // 需要布尔表达式产生返回值
        Temp_label* trueLabel = Temp_newlabel();
        Temp_label* falseLabel = Temp_newlabel();
        Temp_label* exitLabel = Temp_newlabel();

        // 初始化boolVal,alloca
        AS_operand* boolVal = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        emit_irs.emplace_back(L_Alloca(boolVal));
        
        //计算bool 
        ast2llvmBoolExpr(r->u.boolExpr,trueLabel,falseLabel);

        emit_irs.emplace_back(L_Label(trueLabel));
        emit_irs.emplace_back(L_Store(AS_Operand_Const(1),boolVal));
        emit_irs.emplace_back(L_Jump(exitLabel));

        emit_irs.emplace_back(L_Label(falseLabel));
        emit_irs.emplace_back(L_Store(AS_Operand_Const(0),boolVal));
        emit_irs.emplace_back(L_Jump(exitLabel)) ;     

        emit_irs.emplace_back(L_Label(exitLabel));
        AS_operand* dst = AS_Operand_Temp(Temp_newtemp_int());
        emit_irs.emplace_back(L_Load(dst,boolVal));
        return dst;
    }
}

AS_operand* ast2llvmLeftVal(aA_leftVal l)
{
    if (l->kind == A_leftValType::A_varValKind){
        return ast2llvmIdExpr(*l->u.id,true);
    }else if (l->kind == A_leftValType::A_arrValKind){
        return ast2llvmArrayExpr(l->u.arrExpr);
    }else if (l->kind == A_leftValType::A_memberValKind){
        return ast2llvmMemberExpr(l->u.memberExpr);
    }else{
        assert(0); // 难道还有其它的左值类型？
    }
}

AS_operand* ast2llvmIndexExpr(aA_indexExpr index)
{
    if (index->kind == A_indexExprKind::A_numIndexKind){
        return AS_Operand_Const(index->u.num);
    }else if (index->kind == A_indexExprKind::A_idIndexKind){
        return ast2llvmIdExpr(*index->u.id);
    }else{
        assert(0); // 难道还有其它indexExpr
    }
}

AS_operand* ast2llvmBoolExpr(aA_boolExpr b,Temp_label *true_label,Temp_label *false_label)
{
    if (b->kind == A_boolExprType::A_boolBiOpExprKind){
        // 内部针对and or进行优化
        ast2llvmBoolBiOpExpr(b->u.boolBiOpExpr,true_label,false_label);
        return nullptr;
    }else{

        return ast2llvmBoolUnit(b->u.boolUnit,true_label,false_label);
    }
}

void ast2llvmBoolBiOpExpr(aA_boolBiOpExpr b,Temp_label *true_label,Temp_label *false_label)
{
    AS_operand* left = ast2llvmBoolExpr(b->left,true_label,false_label);
    if (b->op == A_boolBiOp::A_and){
        Temp_label* check_right_label = Temp_newlabel();
        // 真则检查右侧，否则直接短路
        emit_irs.emplace_back(L_Cjump(left,check_right_label,false_label));
        emit_irs.emplace_back(L_Label(check_right_label));
         // 检查右侧
        AS_operand* right = ast2llvmBoolExpr(b->right,true_label,false_label);
        if (right != nullptr){
            emit_irs.emplace_back(L_Cjump(right,true_label,false_label));
        }
    }else if (b->op == A_boolBiOp::A_or){
        Temp_label* check_right_label = Temp_newlabel();
        // 真直接跳转，否则检查右侧
        emit_irs.emplace_back(L_Cjump(left,true_label,check_right_label));
        emit_irs.emplace_back(L_Label(check_right_label));
        // 检查右侧
        AS_operand* right = ast2llvmBoolExpr(b->right,true_label,false_label);
        // 如果右侧仍然是biop，那么它自己会jump，不需要你添加jump
        if (right != nullptr){
            emit_irs.emplace_back(L_Cjump(right,true_label,false_label));
        }
    }else{
        assert(0);
    }
}

AS_operand* ast2llvmBoolUnit(aA_boolUnit b,Temp_label *true_label,Temp_label *false_label)
{
    if (b->kind == A_boolUnitType::A_comOpExprKind){
        return ast2llvmComOpExpr(b->u.comExpr,true_label,false_label);
    }else if (b->kind == A_boolUnitType::A_boolExprKind){
        return ast2llvmBoolExpr(b->u.boolExpr,true_label,false_label);
    }else if (b->kind == A_boolUnitType::A_boolUOpExprKind){
        ast2llvmBoolUOpExpr(b->u.boolUOpExpr,true_label,false_label);
        return nullptr; // 不返回值
    }else {
        assert(0);
    }
}

AS_operand* ast2llvmComOpExpr(aA_comExpr c,Temp_label *true_label,Temp_label *false_label)
{
    AS_operand* left_operand = ast2llvmExprUnit(c->left);
    AS_operand* right_operand = ast2llvmExprUnit(c->right);
    // 这里不知道这个类型对不对
    AS_operand* res = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
    if (c->op == A_comOp::A_lt){ 
        emit_irs.emplace_back(L_Cmp(L_relopKind::T_lt,left_operand,right_operand,res));
    }else if (c->op == A_comOp::A_le){
        emit_irs.emplace_back(L_Cmp(L_relopKind::T_le,left_operand,right_operand,res));
    }else if (c->op == A_comOp::A_gt){
        emit_irs.emplace_back(L_Cmp(L_relopKind::T_gt,left_operand,right_operand,res));
    }else if (c->op == A_comOp::A_ge){
        emit_irs.emplace_back(L_Cmp(L_relopKind::T_ge,left_operand,right_operand,res));
    }else if (c->op == A_comOp::A_eq){
        emit_irs.emplace_back(L_Cmp(L_relopKind::T_eq,left_operand,right_operand,res));
    }else if (c->op == A_comOp::A_ne){
        emit_irs.emplace_back(L_Cmp(L_relopKind::T_ne,left_operand,right_operand,res));
    }
    return res;
}
AS_operand* ast2llvmBoolUOpExpr(aA_boolUOpExpr b,Temp_label *true_label,Temp_label *false_label){
    // 条件倒过来
    return  ast2llvmBoolUnit(b->cond,false_label,true_label);
}

AS_operand* ast2llvmArithBiOpExpr(aA_arithBiOpExpr a)
{
    bool changeInFunc = false;
    if (isInFunc){
        // 保证在运算过程中是正常的
        isInFunc = false;
        changeInFunc = true;
    } 
    AS_operand* as_left = ast2llvmArithExpr(a->left);
    AS_operand* as_right = ast2llvmArithExpr(a->right);
    AS_operand* dst = AS_Operand_Temp(Temp_newtemp_int());
    if (changeInFunc){
        isInFunc = true;
    }
    if (a->op ==A_arithBiOp::A_add ){
        emit_irs.emplace_back(L_Binop(L_binopKind::T_plus,as_left,as_right,dst));
    }
    else if (a->op ==A_arithBiOp::A_sub ){
        emit_irs.emplace_back(L_Binop(L_binopKind::T_minus,as_left,as_right,dst));
    }
    else if (a->op ==A_arithBiOp::A_mul ){
        emit_irs.emplace_back(L_Binop(L_binopKind::T_mul,as_left,as_right,dst));
    }
    else if (a->op ==A_arithBiOp::A_div ){
        emit_irs.emplace_back(L_Binop(L_binopKind::T_div,as_left,as_right,dst));
    }
    return dst;
}

AS_operand* ast2llvmArithUExpr(aA_arithUExpr a)
{
    AS_operand* left = AS_Operand_Const(0);
    AS_operand* right = ast2llvmExprUnit(a->expr);
    AS_operand* dst = AS_Operand_Temp(Temp_newtemp_int());
    emit_irs.emplace_back(L_Binop(L_binopKind::T_minus,left,right,dst));
    return dst;
}

AS_operand* ast2llvmArithExpr(aA_arithExpr a)
{
    if (a->kind == A_arithExprType::A_arithBiOpExprKind){
        return ast2llvmArithBiOpExpr(a->u.arithBiOpExpr);
    }else if (a->kind == A_arithExprType::A_exprUnitKind){
        return ast2llvmExprUnit(a->u.exprUnit);
    }
}

AS_operand* ast2llvmExprUnit(aA_exprUnit e)
{
    if (e->kind == A_exprUnitType::A_numExprKind){
        AS_operand* res = AS_Operand_Const(e->u.num);
        return res;
    }
    if (e->kind == A_exprUnitType::A_idExprKind){
       return ast2llvmIdExpr(*e->u.id);
    }
    if (e->kind == A_exprUnitType::A_arithExprKind){
        return ast2llvmArithExpr(e->u.arithExpr);
    }
    if (e->kind == A_exprUnitType::A_fnCallKind){
       return ast2llvmCallExpr(e->u.callExpr);
    }
    // ExprUnit只能是右值
    if (e->kind == A_exprUnitType::A_arrayExprKind){
        return ast2llvmArrayExpr(e->u.arrayExpr,true); // 右值的ArrayExpr
    }
    if (e->kind == A_exprUnitType::A_memberExprKind){
       return ast2llvmMemberExpr(e->u.memberExpr,true); // 右值的MemberExpr
    }
    if (e->kind == A_exprUnitType::A_arithUExprKind){
        return ast2llvmArithUExpr(e->u.arithUExpr);
    }

}

AS_operand* ast2llvmMemberExpr(aA_memberExpr e, bool inRight){
    aA_memberExpr memberExpr = e;
    AS_operand* memberLeftVal = ast2llvmLeftVal(memberExpr->structId);
    // 为了找到属性对应的offset，先找原struct名字
    string structName;
    if (memberLeftVal->kind == OperandKind::TEMP){
        Temp_temp * tmp = memberLeftVal->u.TEMP;
        printf("TEMP structname: %s  \n",tmp->structname.c_str());
        structName = tmp->structname;
    }else if (memberLeftVal->kind == OperandKind::NAME){
        Name_name* name = memberLeftVal->u.NAME;
        printf("NAME structname: %s  \n",name->structname.c_str());
        structName = name->structname;
    }else{
        assert(0); // 不应该是立即数
    }
    // 找到属性名的类型
    string id = *memberExpr->memberId;
    printf("找到Struct %s,属性名%s\n", structName.c_str(),id.c_str());
    MemberInfo si = structInfoMap[structName].memberinfos[id];
    AS_operand* indexOperand = AS_Operand_Const(si.offset);  // 得到偏移量

    AS_operand* ptr ;
    if (si.def.kind == TempType::INT_TEMP ){
        ptr= AS_Operand_Temp(Temp_newtemp_int_ptr(0));
    }else if (si.def.kind == TempType::STRUCT_TEMP  ){
        ptr= AS_Operand_Temp(Temp_newtemp_struct_ptr(0,si.def.structname));
    }else if (si.def.kind == TempType::INT_PTR){
        ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(si.def.len));
    }else if (si.def.kind == TempType::STRUCT_PTR){
        ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(si.def.len,si.def.structname));
    }
    else {
        assert(0); // 不允许数组.prop
    }

    if (!inRight){
        emit_irs.emplace_back(L_Gep(ptr,memberLeftVal,indexOperand));
        return ptr;
    }else{
        // 作为右值时---------------
        // 首先获取member的指针
        emit_irs.emplace_back(L_Gep(ptr,memberLeftVal,indexOperand));
        if(isInFunc){
            // 在函数参数的右值里
            if (si.def.kind == TempType::INT_TEMP){
                // int类型需要load
                AS_operand* dst =  AS_Operand_Temp(Temp_newtemp_int());
                emit_irs.emplace_back(L_Load(dst,ptr));
                return dst;
            }else{
                return ptr;
            }          
        }else{
            // 在运算里，需要load
            AS_operand* dst =  sameTypeButNewTemp(ptr);
            emit_irs.emplace_back(L_Load(dst,ptr));
            return dst;
        }  
    }
}

AS_operand* ast2llvmIdExpr(string id,bool fromLeft){
    string varName = id;
    AS_operand* dst ;
    // ptr是原值
    AS_operand* ptr;
    // 左值 -------------------------------------
    if (fromLeft){
        printf("左值 %s\n",id.c_str());
        dst = getVarNameOperand(id);
        return dst;
    }

    // 右值-------------------------------------
    ptr = getVarNameOperand(id);
    // 非函数，例如运算,需要load值
    if (!isInFunc ){
        dst = AS_Operand_Temp(Temp_newtemp_int());
        emit_irs.emplace_back(L_Load(dst, ptr)); 
        return dst;
    }
    // 函数参数，特殊处理
    if(ptr->kind == OperandKind::NAME){
        // 全局变量，只有int需要load
        if (ptr->u.NAME->type == TempType::INT_TEMP){
            dst = AS_Operand_Temp(Temp_newtemp_int());
            emit_irs.emplace_back(L_Load(dst, ptr)); 
            return dst;
        }else{
            return ptr;
        } 
    }else if (ptr->kind == OperandKind::TEMP){
        // 局部变量，只有int_ptr(0)需要load
        if (ptr->u.TEMP->type == TempType::INT_PTR && ptr->u.TEMP->len == 0){
            dst = AS_Operand_Temp(Temp_newtemp_int());
            emit_irs.emplace_back(L_Load(dst, ptr)); 
            return dst;
        }else{
            return ptr;
        }
    }else{
        assert(0); // id 总不能是立即数
    }
    
   
}

AS_operand* ast2llvmArrayExpr(aA_arrayExpr a, bool inRight){
    aA_arrayExpr arrayExpr = a;
    AS_operand* arrayLeftVal = ast2llvmLeftVal(arrayExpr->arr);
    AS_operand* arrayIdx = ast2llvmIndexExpr(arrayExpr->idx);

    // 作为右值时，取得指针后仍然需要load出来
    if (inRight){
        // 针对右值--------------------------------------
        AS_operand* ptr = sameTypeButNewTemp(arrayLeftVal); // 对应的那个元素的指针
        emit_irs.emplace_back(L_Gep(ptr,arrayLeftVal,arrayIdx));
        AS_operand* dst= AS_Operand_Temp(Temp_newtemp_int()); //返回值，load元素的值
        emit_irs.emplace_back(L_Load(dst,ptr));
        return dst;
    }else{
        // 针对左值--------------------------------------
        AS_operand* ptr;
        if (arrayLeftVal->kind == OperandKind::TEMP){
            if (arrayLeftVal->u.TEMP->type == TempType::INT_PTR){
                ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
            }else if (arrayLeftVal->u.TEMP->type == TempType::STRUCT_PTR){
                ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(0,arrayLeftVal->u.TEMP->structname));
            }else{
                assert(0); // 必须是数组
            }
        }else if (arrayLeftVal->kind == OperandKind::NAME){
           if (arrayLeftVal->u.NAME->type == TempType::INT_PTR){
                ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
            }else if (arrayLeftVal->u.NAME->type == TempType::STRUCT_PTR){
                ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(0,arrayLeftVal->u.NAME->structname));
            }else{
                assert(0); // 必须是数组
            }
        }else {
            assert(0); // 数组怎么能是立即数
        }
        emit_irs.emplace_back(L_Gep(ptr,arrayLeftVal,arrayIdx));
        return ptr;
    }
}

AS_operand* ast2llvmCallExpr(aA_fnCall f){
   
    vector<AS_operand*> args;
    isInFunc = true;
    for (const auto& rval : f->vals){
        args.push_back(ast2llvmRightVal(rval));
    }
    isInFunc = false;
     // 创建临时变量存放ret，
    AS_operand* dst = AS_Operand_Temp(Temp_newtemp_int());
    // 添加fnCall语句 
    emit_irs.emplace_back(L_Call(*f->fn,dst,args));
    return dst;
}



// Func_local* -> L_func
LLVMIR::L_func* ast2llvmFuncBlock(Func_local *f)
{
    list<L_block*> blocks;
    list<L_stm*> tmpIrs;
    for (L_stm* cur: f->irs){
        if(cur->type == L_StmKind::T_LABEL){
            if (tmpIrs.size() != 0 ){
                blocks.push_back(L_Block(tmpIrs));
            }
            tmpIrs.clear();
        }
        tmpIrs.push_back(cur);
    }
    // 结束时再次创建block
    blocks.push_back(L_Block(tmpIrs));
    tmpIrs.clear();
    return new L_func(f->name,f->ret,f->args,blocks);
}

void ast2llvm_moveAlloca(LLVMIR::L_func *f)
{
    list<L_block*>& blocks = f->blocks;
    list<L_stm*>& firstInstrs =f->blocks.front()->instrs;
    
    bool isFirstBlock = true;
    for (L_block* currentBlock : blocks) {
        // 跳过第一个block的alloca
        if (isFirstBlock){
            isFirstBlock = false;
            continue;
        }

        list<L_stm*>& instrs = currentBlock->instrs;
        for (auto it = instrs.begin();it != instrs.end();){
            if ((*it)->type == L_StmKind::T_ALLOCA){
                auto nxt = std::next(firstInstrs.begin(),1);
                firstInstrs.insert(nxt,*it);
                it = instrs.erase(it);
            }else{
                it++;
            }
        }
    }
    
}

AS_operand* sameTypeButNewTemp(AS_operand* x){
    if (x->kind == OperandKind::TEMP){
        if (x->u.TEMP->type == TempType::INT_TEMP){
            assert(0); // 局部变量不应该有int_temp
            return AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        }else if (x->u.TEMP->type == TempType::INT_PTR){
            return AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        }else if (x->u.TEMP->type == TempType::STRUCT_TEMP){
            return AS_Operand_Temp(Temp_newtemp_struct_ptr(0,x->u.TEMP->structname));
        }else if (x->u.TEMP->type == TempType::STRUCT_PTR){
            return AS_Operand_Temp(Temp_newtemp_struct_ptr(0,x->u.TEMP->structname));
        }
    }else if (x->kind == OperandKind::NAME){
        if (x->u.NAME->type == TempType::INT_TEMP){
            return AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        }else if (x->u.NAME->type == TempType::INT_PTR){
            return AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        }else if (x->u.NAME->type == TempType::STRUCT_TEMP){
            return AS_Operand_Temp(Temp_newtemp_struct_ptr(0,x->u.TEMP->structname));
        }
        else if (x->u.NAME->type == TempType::STRUCT_PTR){
            return AS_Operand_Temp(Temp_newtemp_struct_ptr(0,x->u.TEMP->structname));
        }
    }else{
        printf("产生了CONST INT\n");
        assert(0);
    }
}

AS_operand* getVarNameOperand(string varName){
    AS_operand* ptr ;
    if (localVarMap.find(varName) != localVarMap.end()){
        ptr = AS_Operand_Temp(localVarMap[varName]); 
    }else if (globalVarMap.find(varName) != globalVarMap.end()){
        printf("查找全局变量---- %s\n",varName.c_str());
        ptr = AS_Operand_Name(globalVarMap[varName]);
    }else if (funcParamMap.find(varName) != funcParamMap.end()){
        // 函数参数作为右值需要load
        ptr = AS_Operand_Temp(funcParamMap[varName]); 
    } 
    else{
        printf("varname not find %s",varName.c_str());
        assert(0); // 变量可能从未声明
    }
    return ptr;
}
