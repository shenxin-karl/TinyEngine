%code requires {
    #include <memory>
    #include <string>
    #include "ast.h"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "ast.h"

int yylex();
void yyerror(std::unique_ptr<BaseAst> &ast, const char *s);

%}

// 定义 parser 函数的输出参数
// int parser(std::unique_ptr<std::string> &ast);
%parse-param { std::unique_ptr<BaseAst> &ast }

// yylval 的定义, 我们把它定义成了一个联合体 (union)
%union {
    std::string *str_val;
    int int_val;
    BaseAst *ast_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN
%token <str_val> IDENT
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <str_val> FuncType Number
%type <ast_val> FuncDef Block Stmt

%%

CompUnit : FuncDef {
    auto pCompAst = new CompUnitAst();
    pCompAst->pFuncDef = std::unique_ptr<FuncDefAst>(static_cast<FuncDefAst *>($1));
    ast = std::unique_ptr<CompUnitAst>(pCompAst);
};

FuncDef : FuncType IDENT '(' ')' Block {
    auto ast = new FuncDefAst();
    ast->funcType = *$1;
    ast->ident = *$2;
    ast->pBlock = std::unique_ptr<BlockAst>(static_cast<BlockAst *>($5));
    $$ = ast;
};

FuncType : INT {
    $$ = new std::string("int");
};

Block : '{' Stmt '}' {
    auto ast = new BlockAst();
    ast->pStmt = std::unique_ptr<SmitAst>(static_cast<SmitAst *>($2));
    $$ = ast;
};

Stmt : RETURN Number ';' {
    auto ast = new SmitAst();
    ast->returnValue = std::stoi(*$2);
    $$ = ast;
};

Number : INT_CONST {
    $$ = new std::string(std::to_string($1));
};

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(std::unique_ptr<BaseAst> &ast, const char *s) {
    std::cerr << "error: " << s << std::endl;
}