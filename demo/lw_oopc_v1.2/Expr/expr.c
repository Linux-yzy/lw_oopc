#include <stdio.h>
#include "string.h"
#include "Expr.h"

//ABS_CTOR(Expr_node)
void Expr_node_ctor(Expr_node* cthis) {
	cthis->use = 1;             // 构造函数中，将引用计数初始化为1
//END_ABS_CTOR
}
	
// Expr_node的析构函数（DTOR/END_DTOR用于实现析构函数语义）
//DTOR(Expr_node)
void Expr_node_delete(Expr_node* cthis) { 
	if(Expr_node_dtor(cthis)) 
	{ lw_oopc_free(cthis); } 
} 
int Expr_node_dtor(Expr_node* cthis) {
	if (--cthis->use == 0)      // 递减引用计数，如果计数为0，释放自己
	{
		cthis->finalize(cthis); // 释放内存之前先清理资源(其他需要释放的对象）
		return lw_oopc_true;
	}

	return lw_oopc_false;
//END_DTOR
}

// 构建整数表达式（包含一个整数值，无子表达式），n为整数值
void Expr_initInt(Expr* expr, int n)
{
	//Int_node* intNode = Int_node_new(lw_oopc_file_line);
	Int_node* intNode = Int_node *Int_node_new(lw_oopc_file_line);
	intNode->init(intNode, n);
	
	//expr->p = SUPER_PTR(intNode, Expr_node);
	expr->p = ((Expr_node*)(&(intNode->Expr_node)));
}
	
// 构建一元表达式（包含一个操作符，一个子表达式），op为操作符，opnd为子表达式
void Expr_initUnary(Expr* expr, const char* op, Expr* opnd)
{
	//Unary_node* unaryNode = Unary_node_new(lw_oopc_file_line);
	Unary_node* unaryNode = Unary_node *Unary_node_new();

	unaryNode->init(unaryNode, op, opnd);
	//expr->p = SUPER_PTR(unaryNode, Expr_node);
	expr->p = ((Expr_node*)(&(unaryNode->Expr_node)));
}

// 构建一元表达式的重载形式(通过传入一个整型值参数，构造一个子表达式为整数表达式的一元表达式）
// op为操作符，a为子表达式的整型值
void Expr_initUnaryX(Expr* expr, const char* op, int a)
{
	//Expr* intExpr = Expr_new(lw_oopc_file_line);
	Expr* intExpr = Expr *Expr_new(lw_oopc_file_line);
	//Unary_node* unaryNode = Unary_node_new(lw_oopc_file_line);
	Unary_node* unaryNode = Unary_node *Unary_node_new();

	intExpr->initInt(intExpr, a);
	unaryNode->init(unaryNode, op, intExpr);
	//expr->p = SUPER_PTR(unaryNode, Expr_node);
	expr->p = ((Expr_node*)(&(unaryNode->Expr_node)));

	Expr_delete(intExpr);	
}

// 构建二元表达式（包含一个操作符，二个子表达式）
// op为操作符，left为左子表达式，right为右子表达式
void Expr_initBinary(Expr* expr, const char* op, Expr* left, Expr* right)
{
	Binary_node* binaryNode = Binary_node_new(lw_oopc_file_line);
	binaryNode->init(binaryNode, op, left, right);
	expr->p = SUPER_PTR(binaryNode, Expr_node);
}

// 构建二元表达式的重载形式(通过传入两个整型值参数，构造两个子表达式均为整数表达式的二元表达式）
// op为操作符，a为左子表达式的整型值，b为右子表达式的整型值
void Expr_initBinaryX(Expr* expr, const char* op, int a, int b)
{
	Expr* left = Expr_new(lw_oopc_file_line);
	Expr* right = Expr_new(lw_oopc_file_line);
	Binary_node* binaryNode = Binary_node_new(lw_oopc_file_line);

	left->initInt(left, a);
	right->initInt(right, b);

	binaryNode->init(binaryNode, op, left, right);
	expr->p = SUPER_PTR(binaryNode, Expr_node);

	Expr_delete(left);
	Expr_delete(right);
}

// 打印表达式（子树）
void Expr_print(Expr* t)
{
	Expr_node* p = t->p;
	p->print(p);
}

//CTOR(Expr)
Expr* Expr_new() { 
	struct Expr *cthis; 
	cthis = (struct Expr*)malloc(sizeof(struct Expr)); 
	if(!cthis) { return 0; } Expr_ctor(cthis); return cthis; 
}
void Expr_ctor(Expr* cthis) {
//FUNCTION_SETTING(initInt, Expr_initInt);
cthis->initInt = Expr_initInt;
//FUNCTION_SETTING(initUnary, Expr_initUnary);
cthis->initUnary = Expr_initUnary;
//FUNCTION_SETTING(initUnaryX, Expr_initUnaryX);
cthis->initUnaryX = Expr_initUnaryX;
//FUNCTION_SETTING(initBinary, Expr_initBinary);
cthis->initBinary = Expr_initBinary;
//FUNCTION_SETTING(initBinaryX, Expr_initBinaryX);
cthis->initBinaryX = Expr_initBinaryX;
//FUNCTION_SETTING(print, Expr_print);
cthis->print = Expr_print;
	cthis->use = 1;             // 构造函数中，将引用计数初始化为1
//END_CTOR
}

// Expr的析构函数（DTOR/END_DTOR用于实现析构函数语义）
//DTOR(Expr)
void Expr_delete(Expr* cthis) { 
	if(Expr_dtor(cthis)) { 
		lw_oopc_free(cthis); 
		} 
	} 
int Expr_dtor(Expr* cthis) {
	if (--cthis->use == 0)      // 递减引用计数，如果计数为0，释放自己
	{
		Expr_node_delete(cthis->p);
		return lw_oopc_true;
	}

	return lw_oopc_false;
//END_DTOR
}

// 整数表达式节点的初始化
void Int_node_init(Int_node* t, int k)
{
	t->n = k;
}

// 整数表达式节点的打印
void Int_node_print(Expr_node* t) 
{
	Int_node* cthis = SUB_PTR(t, Expr_node, Int_node);
	printf("%d", cthis->n); 
}

// 整数表达式节点的资源清理
void Int_node_finalize(Expr_node* t)
{
	// 什么都不需要做
}

//CTOR(Int_node)
Int_node* Int_node_new() { 
	struct Int_node *cthis; 
	cthis = (struct Int_node*)malloc(sizeof(struct Int_node)); 
	if(!cthis) { return 0; } Int_node_ctor(cthis); return cthis; 
} 
void Int_node_ctor(Int_node* cthis) {
//SUPER_CTOR(Expr_node);
Expr_node_ctor(((Expr_node*)(&(cthis->Expr_node))));
FUNCTION_SETTING(init, Int_node_init);
FUNCTION_SETTING(Expr_node.print, Int_node_print);
FUNCTION_SETTING(Expr_node.finalize, Int_node_finalize);
//END_CTOR
}

// 设置节点的操作符
void setOp(char* opAddr, const char* opValue)
{
	memset(opAddr, 0, 3);
	strncpy(opAddr, opValue, 2);
}

// 一元表达式节点的初始化
void Unary_node_init(Unary_node* t, const char* opValue, Expr* b)
{
	setOp(t->op, opValue);
	t->opnd = b;
	++b->use;   // 指针赋值时，将指针所指对象的引用计数自增
}

// 一元表达式节点的打印
void Unary_node_print(Expr_node* t)
{
	Unary_node* cthis = SUB_PTR(t, Expr_node, Unary_node);
	Expr* opnd = cthis->opnd;

	printf("(");
	printf("%s", cthis->op);
	opnd->print(opnd);
	printf(")"); 
}

// 一元表达式节点的资源清理
void Unary_node_finalize(Expr_node* t)
{
	Unary_node* cthis = SUB_PTR(t, Expr_node, Unary_node);

	Expr_delete(cthis->opnd);
}

CTOR(Unary_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Unary_node_init);
FUNCTION_SETTING(Expr_node.print, Unary_node_print);
FUNCTION_SETTING(Expr_node.finalize, Unary_node_finalize);
END_CTOR

// 二元表达式节点的初始化
void Binary_node_init(Binary_node* t, const char* opValue, Expr* left, Expr* right)
{
    setOp(t->op, opValue);
	t->left = left;
	t->right = right;
	++left->use;    // 指针赋值时，将指针所指对象的引用计数自增
	++right->use;   // 指针赋值时，将指针所指对象的引用计数自增
}

// 二元表达式节点的打印
void Binary_node_print(Expr_node* t)
{
	Binary_node* cthis = SUB_PTR(t, Expr_node, Binary_node);

	Expr* left = cthis->left;
	Expr* right = cthis->right;

	printf("(");
	left->print(left);
	printf("%s", cthis->op);
	right->print(right);
	printf(")"); 
}

// 二元表达式节点的资源清理
void Binary_node_finalize(Expr_node* t)
{
	Binary_node* cthis = SUB_PTR(t, Expr_node, Binary_node);

	Expr_delete(cthis->left);
	Expr_delete(cthis->right);
}

CTOR(Binary_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Binary_node_init);
FUNCTION_SETTING(Expr_node.print, Binary_node_print);
FUNCTION_SETTING(Expr_node.finalize, Binary_node_finalize);
END_CTOR