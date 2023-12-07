%{
#include <stdio.h>
#include "TeaplAst.h"

extern A_pos pos;
extern A_program root;

extern int yylex(void);
extern "C"{
extern void yyerror(char *s); 
extern int  yywrap();
}

%}

// TODO:
// your parser



%union {
  A_pos pos;
  A_program program;
  A_programElementList programElementList;
  A_programElement programElement;
  A_arithExpr arithExpr;
  A_exprUnit exprUnit;
  A_structDef structDef;
  A_varDeclStmt varDeclStmt;
  A_fnDeclStmt fnDeclStmt;
  A_fnDef fnDef;
  
  // newly add
  A_tokenNum tokenNum;
  A_tokenId tokenId;
  A_fnCall fnCall;
  A_boolExpr boolExpr;
  A_boolUnit boolUnit;
  A_assignStmt assignStmt;
  A_rightVal rightVal;
  A_leftVal leftVal;
  A_rightValList rightValList;
  A_varDecl varDecl;
  A_varDeclList varDeclList;
  A_varDef varDef;
  A_varDeclScalar varDeclScalar;
  A_varDeclArray varDeclArray;
  A_varDefScalar varDefScalar;
  A_varDefArray varDefArray;
  A_type type;
  // A_nativeType nativeType;
  A_fnDecl fnDecl;
  A_paramDecl paramDecl;
  // FnDef
  A_codeBlockStmtList codeBlockStmtList;
  A_codeBlockStmt codeBlockStmt;
  A_callStmt callStmt;
  A_ifStmt ifStmt;
  A_whileStmt whileStmt;
  A_returnStmt returnStmt;
}

%token <pos> ADD
%token <pos> SUB
%token <pos> MUL
%token <pos> DIV
%token <pos> SEMICOLON 
// newly add
%token <pos> DOT;
%token <pos> LBRACE;
%token <pos> RBRACE;
%token <pos> LPAR;
%token <pos> RPAR;
%token <pos> LBRACKET;
%token <pos> RBRACKET;
%token <tokenNum> UNUM;
%token <tokenId> ID;


%token <pos> AND;
%token <pos> OR;
%token <pos> NOT;
%token <pos> GTR;
%token <pos> LES;
%token <pos> GEQ;
%token <pos> LEQ;
%token <pos> EQL;
%token <pos> NEQ;

%token <pos> INT;
%token <pos> LET;
%token <pos> COLON;
%token <pos> ASSIGN;
%token <pos> STRUCT;
%token <pos> COMMA;
%token <pos> RARROW

%token <pos> IF
%token <pos> ELSE
%token <pos> RETURN
%token <pos> WHILE
%token <pos> CONTINUE
%token <pos> BREAK
%token <pos> FN

%type <program> Program
%type <arithExpr> ArithExpr
%type <programElementList> ProgramElementList
%type <programElement> ProgramElement
%type <exprUnit> ExprUnit
%type <structDef> StructDef
%type <varDeclStmt> VarDeclStmt
%type <fnDeclStmt> FnDeclStmt
%type <fnDef> FnDef
// newly add
%type <tokenNum> Num
%type <tokenId> Id
%type <fnCall> FnCall
%type <boolExpr> BoolExpr
%type <boolUnit> BoolUnit
%type <assignStmt> AssignStmt
%type <rightValList> RightValList
%type <rightVal> RightVal
%type <leftVal> LeftVal
%type <varDecl> VarDecl
%type <varDef> VarDef
%type <varDeclList> VarDeclList
%type <varDeclScalar> VarDeclScalar
%type <varDeclArray> VarDeclArray
%type <varDefScalar> VarDefScalar
%type <varDefArray> VarDefArray
%type <type> Type
// %type <nativeType> NType

%type <fnDecl> FnDecl
%type <paramDecl> ParamDecl

%type <codeBlockStmtList> CodeBlockStmtList
%type <codeBlockStmt> CodeBlockStmt
%type <callStmt> CallStmt
%type <ifStmt> IfStmt
%type <whileStmt> WhileStmt
%type <returnStmt> ReturnStmt

// 声明优先级
%left ADD SUB
%left MUL DIV

%start Program

%%                   /* beginning of rules section */

Program: ProgramElementList 
{  
  root = A_Program($1);
  $$ = A_Program($1);
}
;

ProgramElementList: ProgramElement ProgramElementList
{
  $$ = A_ProgramElementList($1, $2);
}
|
{
  $$ = NULL;
}
;

// Still lack of comment
ProgramElement: VarDeclStmt
{
  $$ = A_ProgramVarDeclStmt($1->pos, $1);
}
| StructDef
{
  $$ = A_ProgramStructDef($1->pos, $1);
}
| FnDeclStmt
{
  $$ = A_ProgramFnDeclStmt($1->pos, $1);
}
| FnDef
{
  $$ = A_ProgramFnDef($1->pos, $1);
}
| SEMICOLON
{
  $$ = A_ProgramNullStmt($1);
}
;


ArithExpr: ArithExpr ADD ArithExpr
{
  $$ = A_ArithBiOp_Expr($1->pos, A_ArithBiOpExpr($1->pos, A_add, $1, $3));
}
| ArithExpr SUB ArithExpr
{
  $$ = A_ArithBiOp_Expr($1->pos, A_ArithBiOpExpr($1->pos, A_sub, $1, $3));
}
| ArithExpr MUL ArithExpr
{
  $$ = A_ArithBiOp_Expr($1->pos, A_ArithBiOpExpr($1->pos, A_mul, $1, $3));
}
| ArithExpr DIV ArithExpr
{
  $$ = A_ArithBiOp_Expr($1->pos, A_ArithBiOpExpr($1->pos, A_div, $1, $3));
}
| ExprUnit
{
  $$ = A_ExprUnit($1->pos, $1);
}
;
// newly add
ExprUnit: Num
{
  $$ = A_NumExprUnit($1->pos,$1->num);
}
| Id
{

  $$ = A_IdExprUnit($1->pos, $1->id);
}
| LPAR ArithExpr RPAR
{
  $$ = A_ArithExprUnit($1, $2);
}
| FnCall
{
  $$ = A_CallExprUnit($1->pos ,$1);
}
| LeftVal LBRACKET Id RBRACKET
{
  
  $$ = A_ArrayExprUnit($1->pos , A_ArrayExpr($1->pos, $1, A_IdIndexExpr($3->pos,$3->id) ));
}
| LeftVal LBRACKET Num RBRACKET
{
  $$ = A_ArrayExprUnit($1->pos , A_ArrayExpr($1->pos, $1, A_NumIndexExpr($3->pos,$3->num) ));
}
| LeftVal DOT Id
{
  $$ = A_MemberExprUnit($1->pos, A_MemberExpr($1->pos,$1,$3->id) );
}
| SUB ExprUnit
{
  $$ = A_ArithUExprUnit($1, A_ArithUExpr($2->pos,A_neg,$2));
};

// Condition Expression
BoolExpr: BoolExpr AND BoolExpr
{
  $$ = A_BoolBiOp_Expr($1->pos, A_BoolBiOpExpr($3->pos,A_and,$1,$3));
}
| BoolExpr OR BoolExpr
{
  $$ = A_BoolBiOp_Expr($1->pos, A_BoolBiOpExpr($3->pos,A_or,$1,$3));
}
| BoolUnit
{
  $$ = A_BoolExpr($1->pos,$1);
};

BoolUnit: ExprUnit GTR ExprUnit
{
  $$ = A_ComExprUnit($1->pos ,A_ComExpr($3->pos,A_gt,$1,$3));
}
| ExprUnit LES ExprUnit
{
  $$ = A_ComExprUnit($1->pos ,A_ComExpr($3->pos,A_lt,$1,$3));
}
| ExprUnit GEQ ExprUnit
{
  $$ = A_ComExprUnit($1->pos ,A_ComExpr($3->pos,A_ge,$1,$3));
}
| ExprUnit LEQ ExprUnit
{
  $$ = A_ComExprUnit($1->pos ,A_ComExpr($3->pos,A_le,$1,$3));
}
| ExprUnit EQL ExprUnit
{
  $$ = A_ComExprUnit($1->pos ,A_ComExpr($3->pos,A_eq,$1,$3));
}
| ExprUnit NEQ ExprUnit
{
  $$ = A_ComExprUnit($1->pos ,A_ComExpr($3->pos,A_ne,$1,$3));
}
| LPAR BoolExpr RPAR
{
  $$ = A_BoolExprUnit($1, $2);
}
| NOT BoolUnit
{
  $$ = A_BoolUOpExprUnit($1,A_BoolUOpExpr($1,A_not,$2));
};


// Assignment & Right Left Val--------------------------------------------------
AssignStmt: LeftVal ASSIGN RightVal SEMICOLON
{
  $$ = A_AssignStmt($1->pos,$1,$3);
};

RightVal: ArithExpr
{
  $$ = A_ArithExprRVal($1->pos,$1);
}
| BoolExpr
{
  $$ = A_BoolExprRVal($1->pos,$1);
};

LeftVal:Id
{
  $$ = A_IdExprLVal($1->pos,$1->id);
}
| LeftVal LBRACKET Id RBRACKET
{
  
  $$ = A_ArrExprLVal($1->pos,A_ArrayExpr($1->pos,$1,A_IdIndexExpr($3->pos,$3->id)));
}
| LeftVal LBRACKET Num RBRACKET
{
  $$ = A_ArrExprLVal($1->pos,A_ArrayExpr($1->pos,$1 ,A_NumIndexExpr($3->pos,$3->num)));
}
| LeftVal DOT Id
{
  $$ = A_MemberExprLVal($1->pos, A_MemberExpr($1->pos,$1,$3->id));
}

// Function Call ------------------------------------------------
FnCall: Id LPAR RightValList RPAR
{
  $$ = A_FnCall($1->pos, $1->id, $3);
};

RightValList: RightVal
{
  $$ = A_RightValList($1,NULL);
}
| RightVal COMMA RightValList
{
  $$ = A_RightValList($1,$3);
}
|
{
  $$ = NULL;
};


// Declaration -------------------------------------------------
VarDeclStmt : LET VarDecl SEMICOLON
{
  $$ = A_VarDeclStmt($1,$2);
}
| LET VarDef SEMICOLON
{
  $$ = A_VarDefStmt($1 , $2);
};

VarDecl: VarDeclScalar
{
  $$ =  A_VarDecl_Scalar($1->pos,$1);
}
| VarDeclArray
{
  $$ = A_VarDecl_Array($1->pos,$1);
};

VarDeclList: VarDecl
{
  $$ = A_VarDeclList($1,NULL);
}
| VarDecl COMMA VarDeclList
{
  $$ = A_VarDeclList($1 ,$3);
}
|
{
  $$ = NULL;
};

VarDeclScalar: Id COLON Type
{
  //printf("match long\n");
  $$ = A_VarDeclScalar($1->pos , $1->id ,$3);
}
| Id 
{
  //printf("match id\n");
  $$ = A_VarDeclScalar($1->pos,$1->id, NULL);
};
 

VarDeclArray: Id LBRACKET Num RBRACKET 
{
  $$ = A_VarDeclArray($1->pos , $1->id, $3->num, NULL);
}
| Id LBRACKET Num RBRACKET COLON Type
{
  $$ = A_VarDeclArray($1->pos , $1->id, $3->num,$6 );
};
 
VarDef: VarDefScalar
{
  $$ = A_VarDef_Scalar($1->pos, $1);
}
| VarDefArray
{
  $$ = A_VarDef_Array($1->pos,$1);
};

VarDefScalar:Id COLON Type ASSIGN RightVal
{
  $$ = A_VarDefScalar($1->pos,$1->id,$3,$5);
}
| Id ASSIGN RightVal
{
  $$ = A_VarDefScalar($1->pos,$1->id,NULL,$3);
};

VarDefArray:Id LBRACKET Num RBRACKET COLON Type ASSIGN LBRACE RightValList RBRACE
{
  $$ = A_VarDefArray($1->pos,$1->id,$3->num,$6,$9);
}
| Id LBRACKET Num RBRACKET ASSIGN LBRACE RightValList RBRACE
{
  $$ = A_VarDefArray($1->pos,$1->id,$3->num,NULL,$7);
};


Type : INT
{
  $$ = A_NativeType($1, A_intTypeKind);
}
| Id
{
  $$ = A_StructType($1->pos, $1->id);
};




// New Structure  -------------------------------------------------
StructDef : STRUCT Id LBRACE VarDeclList RBRACE
{
  $$ = A_StructDef($1, $2->id, $4);
};


//Function Declaration and Definition -------------------------------------------------
FnDeclStmt: FnDecl SEMICOLON
{
  $$ = A_FnDeclStmt($1->pos,$1);
};

FnDecl: FN Id LPAR ParamDecl RPAR RARROW Type
{
  $$ = A_FnDecl($1,$2->id,$4,$7);
};
| FN Id LPAR ParamDecl RPAR
{
  $$ = A_FnDecl($1,$2->id,$4,NULL);
}

ParamDecl: VarDeclList
{
  $$ = A_ParamDecl($1);
};

// Function Definition ---------------------------------------
FnDef : FnDecl LBRACE CodeBlockStmtList RBRACE
{
  $$ = A_FnDef($1->pos,$1, $3);
};
CodeBlockStmtList: CodeBlockStmt 
{
  $$ = A_CodeBlockStmtList($1, NULL);
}
| CodeBlockStmt CodeBlockStmtList
{
  $$ = A_CodeBlockStmtList($1, $2);
}
|
{
  $$ = NULL;
};

CodeBlockStmt: VarDeclStmt
{
  $$ = A_BlockVarDeclStmt($1->pos,$1);
}
| AssignStmt
{
  $$ = A_BlockAssignStmt($1->pos ,$1);
}
| CallStmt
{
  $$ = A_BlockCallStmt($1->pos ,$1);
}
| IfStmt
{
  $$ = A_BlockIfStmt($1->pos ,$1);
}
| WhileStmt
{
  $$ = A_BlockWhileStmt($1->pos ,$1);
}
| ReturnStmt
{
  $$ = A_BlockReturnStmt($1->pos ,$1);
}
| CONTINUE SEMICOLON
{
  $$ = A_BlockContinueStmt($1);
}
| BREAK SEMICOLON
{
  $$ = A_BlockBreakStmt($1);
}
| SEMICOLON
{
  $$ = A_BlockNullStmt($1);
};

ReturnStmt: RETURN RightVal SEMICOLON
{
  $$ = A_ReturnStmt($1 , $2);
}
| RETURN SEMICOLON
{
  $$ = A_ReturnStmt($1 , NULL);
};

CallStmt: FnCall SEMICOLON
{
  $$ = A_CallStmt($1->pos,$1);
};

// Control Flow: If and While

IfStmt: IF LPAR BoolExpr RPAR LBRACE CodeBlockStmtList RBRACE
{
  $$ = A_IfStmt($1, $3, $6,NULL);
}
| IF LPAR BoolExpr RPAR LBRACE CodeBlockStmtList RBRACE ELSE LBRACE CodeBlockStmtList RBRACE
{
  $$ = A_IfStmt($1, $3, $6, $10);
};

WhileStmt: WHILE LPAR BoolExpr RPAR LBRACE CodeBlockStmtList RBRACE
{
  $$ = A_WhileStmt($1, $3, $6);
}

// ID and Num -------------------------------------------------
Num : UNUM
{
  $$ = $1;
};

Id : ID
{
  $$ = $1;
}
%%

extern "C"{
void yyerror(char * s)
{
  fprintf(stderr, "%s\n",s);
}
int yywrap()
{
  return(1);
}
}


