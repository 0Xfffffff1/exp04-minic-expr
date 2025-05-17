///
/// @file IRGenerator.cpp
/// @brief AST遍历产生线性IR的源文件
/// @author zenglj (zenglj@live.com)
/// @version 1.1
/// @date 2024-11-23
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// <tr><td>2024-11-23 <td>1.1     <td>zenglj  <td>表达式版增强
/// </table>
///
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "AST.h"
#include "Common.h"
#include "Function.h"
#include "IRCode.h"
#include "IRGenerator.h"
#include "Module.h"
#include "Instructions/EntryInstruction.h"
#include "Instructions/LabelInstruction.h"
#include "Instructions/ExitInstruction.h"
#include "Instructions/FuncCallInstruction.h"
#include "Instructions/BinaryInstruction.h"
#include "Instructions/MoveInstruction.h"
#include "Instructions/GotoInstruction.h"
/// @brief 构造函数
/// @param _root AST的根
/// @param _module 符号表
IRGenerator::IRGenerator(ast_node * _root, Module * _module) : root(_root), module(_module)
{
    /* 叶子节点 */
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_UINT] = &IRGenerator::ir_leaf_node_uint;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_VAR_ID] = &IRGenerator::ir_leaf_node_var_id;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_TYPE] = &IRGenerator::ir_leaf_node_type;

    /* 表达式运算， 加减 */
    ast2ir_handlers[ast_operator_type::AST_OP_SUB] = &IRGenerator::ir_sub;
    ast2ir_handlers[ast_operator_type::AST_OP_ADD] = &IRGenerator::ir_add;
    ast2ir_handlers[ast_operator_type::AST_OP_MUL] = &IRGenerator::ir_mul;
    ast2ir_handlers[ast_operator_type::AST_OP_DIV] = &IRGenerator::ir_div;
    ast2ir_handlers[ast_operator_type::AST_OP_MOD] = &IRGenerator::ir_mod;
    ast2ir_handlers[ast_operator_type::AST_OP_UNARY_MINUS] = &IRGenerator::ir_unary_minus;

    /* 关系运算符 */
    ast2ir_handlers[ast_operator_type::AST_OP_GT] = &IRGenerator::ir_gt;
    ast2ir_handlers[ast_operator_type::AST_OP_GE] = &IRGenerator::ir_ge;
    ast2ir_handlers[ast_operator_type::AST_OP_LT] = &IRGenerator::ir_lt;
    ast2ir_handlers[ast_operator_type::AST_OP_LE] = &IRGenerator::ir_le;
    ast2ir_handlers[ast_operator_type::AST_OP_EQ] = &IRGenerator::ir_eq;
    ast2ir_handlers[ast_operator_type::AST_OP_NEQ] = &IRGenerator::ir_ne;

    /* 逻辑运算符 */
    ast2ir_handlers[ast_operator_type::AST_OP_LOGICAL_AND] = &IRGenerator::ir_and;
    ast2ir_handlers[ast_operator_type::AST_OP_LOGICAL_OR] = &IRGenerator::ir_or;
    ast2ir_handlers[ast_operator_type::AST_OP_LOGICAL_NOT] = &IRGenerator::ir_not;

    /* 语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_ASSIGN] = &IRGenerator::ir_assign;
    ast2ir_handlers[ast_operator_type::AST_OP_RETURN] = &IRGenerator::ir_return;

    /* 函数调用 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_CALL] = &IRGenerator::ir_function_call;

    /* 函数定义 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_DEF] = &IRGenerator::ir_function_define;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS] = &IRGenerator::ir_function_formal_params;

    /* 变量定义语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_DECL_STMT] = &IRGenerator::ir_declare_statment;
    ast2ir_handlers[ast_operator_type::AST_OP_VAR_DECL] = &IRGenerator::ir_variable_declare;

    /* 语句块 */
    ast2ir_handlers[ast_operator_type::AST_OP_BLOCK] = &IRGenerator::ir_block;

    /* 控制流语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_IF] = &IRGenerator::ir_if;
    ast2ir_handlers[ast_operator_type::AST_OP_IF_ELSE] = &IRGenerator::ir_if_else;
    ast2ir_handlers[ast_operator_type::AST_OP_WHILE] = &IRGenerator::ir_while;
    ast2ir_handlers[ast_operator_type::AST_OP_BREAK] = &IRGenerator::ir_break;
    ast2ir_handlers[ast_operator_type::AST_OP_CONTINUE] = &IRGenerator::ir_continue;

    /* 编译单元 */
    ast2ir_handlers[ast_operator_type::AST_OP_COMPILE_UNIT] = &IRGenerator::ir_compile_unit;
}

/// @brief 遍历抽象语法树产生线性IR，保存到IRCode中
/// @param root 抽象语法树
/// @param IRCode 线性IR
/// @return true: 成功 false: 失败
bool IRGenerator::run()
{
    ast_node * node;

    // 从根节点进行遍历
    node = ir_visit_ast_node(root);

    return node != nullptr;
}

/// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
/// @param node AST节点
/// @return 成功返回node节点，否则返回nullptr
ast_node * IRGenerator::ir_visit_ast_node(ast_node * node)
{
    // 空节点
    if (nullptr == node) {
        return nullptr;
    }

    bool result;

    std::unordered_map<ast_operator_type, ast2ir_handler_t>::const_iterator pIter;
    pIter = ast2ir_handlers.find(node->node_type);
    if (pIter == ast2ir_handlers.end()) {
        // 没有找到，则说明当前不支持
        result = (this->ir_default)(node);
    } else {
        result = (this->*(pIter->second))(node);
    }

    if (!result) {
        // 语义解析错误，则出错返回
        node = nullptr;
    }

    return node;
}

/// @brief 未知节点类型的节点处理
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_default(ast_node * node)
{
    // 未知的节点
    printf("Unkown node(%d)\n", (int) node->node_type);
    return true;
}

/// @brief 编译单元AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_compile_unit(ast_node * node)
{
    module->setCurrentFunction(nullptr);

    for (auto son: node->sons) {

        // 遍历编译单元，要么是函数定义，要么是语句
        ast_node * son_node = ir_visit_ast_node(son);
        if (!son_node) {
            // TODO 自行追加语义错误处理
            return false;
        }
    }

    return true;
}

/// @brief 函数定义AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_define(ast_node * node)
{
    bool result;

    // 创建一个函数，用于当前函数处理
    if (module->getCurrentFunction()) {
        // 函数中嵌套定义函数，这是不允许的，错误退出
        // TODO 自行追加语义错误处理
        return false;
    }

    // 函数定义的AST包含四个孩子
    // 第一个孩子：函数返回类型
    // 第二个孩子：函数名字
    // 第三个孩子：形参列表
    // 第四个孩子：函数体即block
    ast_node * type_node = node->sons[0];
    ast_node * name_node = node->sons[1];
    ast_node * param_node = node->sons[2];
    ast_node * block_node = node->sons[3];

    // 创建一个新的函数定义
    Function * newFunc = module->newFunction(name_node->name, type_node->type);
    if (!newFunc) {
        // 新定义的函数已经存在，则失败返回。
        // TODO 自行追加语义错误处理
        return false;
    }

    // 当前函数设置有效，变更为当前的函数
    module->setCurrentFunction(newFunc);

    // 进入函数的作用域
    module->enterScope();

    // 获取函数的IR代码列表，用于后面追加指令用，注意这里用的是引用传值
    InterCode & irCode = newFunc->getInterCode();

    // 这里也可增加一个函数入口Label指令，便于后续基本块划分

    // 创建并加入Entry入口指令
    irCode.addInst(new EntryInstruction(newFunc));

    // 创建出口指令并不加入出口指令，等函数内的指令处理完毕后加入出口指令
    LabelInstruction * exitLabelInst = new LabelInstruction(newFunc);

    // 函数出口指令保存到函数信息中，因为在语义分析函数体时return语句需要跳转到函数尾部，需要这个label指令
    newFunc->setExitLabel(exitLabelInst);

    // 遍历形参，没有IR指令，不需要追加
    result = ir_function_formal_params(param_node);
    if (!result) {
        // 形参解析失败
        // TODO 自行追加语义错误处理
        return false;
    }
    node->blockInsts.addInst(param_node->blockInsts);

    // 新建一个Value，用于保存函数的返回值，如果没有返回值可不用申请
    LocalVariable * retValue = nullptr;
    if (!type_node->type->isVoidType()) {

        // 保存函数返回值变量到函数信息中，在return语句翻译时需要设置值到这个变量中
        retValue = static_cast<LocalVariable *>(module->newVarValue(type_node->type));
    }
    newFunc->setReturnValue(retValue);

    // 这里最好设置返回值变量的初值为0，以便在没有返回值时能够返回0

    // 函数内已经进入作用域，内部不再需要做变量的作用域管理
    block_node->needScope = false;

    // 遍历block
    result = ir_block(block_node);
    if (!result) {
        // block解析失败
        // TODO 自行追加语义错误处理
        return false;
    }

    // IR指令追加到当前的节点中
    node->blockInsts.addInst(block_node->blockInsts);

    // 此时，所有指令都加入到当前函数中，也就是node->blockInsts

    // node节点的指令移动到函数的IR指令列表中
    irCode.addInst(node->blockInsts);

    // 添加函数出口Label指令，主要用于return语句跳转到这里进行函数的退出
    irCode.addInst(exitLabelInst);

    // 函数出口指令
    irCode.addInst(new ExitInstruction(newFunc, retValue));

    // 恢复成外部函数
    module->setCurrentFunction(nullptr);

    // 退出函数的作用域
    module->leaveScope();

    return true;
}

/// @brief 形式参数AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_formal_params(ast_node * node)
{
    if (!node || node->sons.empty()) {
        return true;
    }

    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        return false;
    }

    for (auto param : node->sons) {
        if (!param || param->sons.size() < 2) {
            return false;
        }

        // 获取参数类型和名称
        Type * paramType = param->sons[0]->type;
        std::string paramName = param->sons[1]->name;

        // 创建参数变量
        LocalVariable * paramValue = currentFunc->newLocalVarValue(paramType, paramName);
        if (!paramValue) {
            return false;
        }

        // 创建形式参数
        FormalParam * formalParam = new FormalParam(paramType, paramName);
        currentFunc->getParams().push_back(formalParam);
    }

    return true;
}

/// @brief 函数调用AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_call(ast_node * node)
{
    std::vector<Value *> realParams;

    // 获取当前正在处理的函数
    Function * currentFunc = module->getCurrentFunction();

    // 函数调用的节点包含两个节点：
    // 第一个节点：函数名节点
    // 第二个节点：实参列表节点

    std::string funcName = node->sons[0]->name;
    int64_t lineno = node->sons[0]->line_no;

    ast_node * paramsNode = node->sons[1];

    // 根据函数名查找函数，看是否存在。若不存在则出错
    // 这里约定函数必须先定义后使用
    auto calledFunction = module->findFunction(funcName);
    if (nullptr == calledFunction) {
        minic_log(LOG_ERROR, "函数(%s)未定义或声明", funcName.c_str());
        return false;
    }

    // 当前函数存在函数调用
    currentFunc->setExistFuncCall(true);

    // 如果没有孩子，也认为是没有参数
    if (!paramsNode->sons.empty()) {

        int32_t argsCount = (int32_t) paramsNode->sons.size();

        // 当前函数中调用函数实参个数最大值统计，实际上是统计实参传参需在栈中分配的大小
        // 因为目前的语言支持的int和float都是四字节的，只统计个数即可
        if (argsCount > currentFunc->getMaxFuncCallArgCnt()) {
            currentFunc->setMaxFuncCallArgCnt(argsCount);
        }

        // 遍历参数列表，孩子是表达式
        // 这里自左往右计算表达式
        for (auto son: paramsNode->sons) {

            // 遍历Block的每个语句，进行显示或者运算
            ast_node * temp = ir_visit_ast_node(son);
            if (!temp) {
                return false;
            }

            realParams.push_back(temp->val);
            node->blockInsts.addInst(temp->blockInsts);
        }
    }

    // TODO 这里请追加函数调用的语义错误检查，这里只进行了函数参数的个数检查等，其它请自行追加。
    if (realParams.size() != calledFunction->getParams().size()) {
        // 函数参数的个数不一致，语义错误
        minic_log(LOG_ERROR, "第%lld行的被调用函数(%s)未定义或声明", (long long) lineno, funcName.c_str());
        return false;
    }

    // 返回调用有返回值，则需要分配临时变量，用于保存函数调用的返回值
    Type * type = calledFunction->getReturnType();

    FuncCallInstruction * funcCallInst = new FuncCallInstruction(currentFunc, calledFunction, realParams, type);

    // 创建函数调用指令
    node->blockInsts.addInst(funcCallInst);

    // 函数调用结果Value保存到node中，可能为空，上层节点可利用这个值
    node->val = funcCallInst;

    return true;
}

/// @brief 语句块（含函数体）AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_block(ast_node * node)
{
    // 进入作用域
    if (node->needScope) {
        module->enterScope();
    }

    std::vector<ast_node *>::iterator pIter;
    for (pIter = node->sons.begin(); pIter != node->sons.end(); ++pIter) {

        // 遍历Block的每个语句，进行显示或者运算
        ast_node * temp = ir_visit_ast_node(*pIter);
        if (!temp) {
            return false;
        }

        node->blockInsts.addInst(temp->blockInsts);
    }

    // 离开作用域
    if (node->needScope) {
        module->leaveScope();
    }

    return true;
}

/// @brief 整数加法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_add(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * addInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_ADD_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(addInst);

    node->val = addInst;

    return true;
}

/// @brief 整数减法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_sub(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * subInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_SUB_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(subInst);

    node->val = subInst;

    return true;
}

/// @brief 整数乘法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mul(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 乘法节点，左结合，先计算左节点，后计算右节点

    // 乘法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 乘法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * mulInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_MUL_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(mulInst);

    node->val = mulInst;

    return true;
}

/// @brief 整数除法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_div(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 除法节点，左结合，先计算左节点，后计算右节点

    // 除法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 除法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * divInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_DIV_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(divInst);

    node->val = divInst;

    return true;
}

/// @brief 整数求余AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mod(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 求余节点，左结合，先计算左节点，后计算右节点

    // 求余的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 求余的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * modInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_MOD_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(modInst);

    node->val = modInst;

    return true;
}

/// @brief 赋值AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_assign(ast_node * node)
{
    ast_node * son1_node = node->sons[0];
    ast_node * son2_node = node->sons[1];

    // 赋值节点，自右往左运算

    // 赋值运算符的左侧操作数
    ast_node * left = ir_visit_ast_node(son1_node);
    if (!left) {
        // 某个变量没有定值
        // 这里缺省设置变量不存在则创建，因此这里不会错误
        return false;
    }

    // 赋值运算符的右侧操作数
    ast_node * right = ir_visit_ast_node(son2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    MoveInstruction * movInst = new MoveInstruction(module->getCurrentFunction(), left->val, right->val);

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(movInst);

    // 这里假定赋值的类型是一致的
    node->val = movInst;

    return true;
}

/// @brief return节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_return(ast_node * node)
{
    ast_node * right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {

        ast_node * son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {

            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    Function * currentFunc = module->getCurrentFunction();

    // 返回值存在时则移动指令到node中
    if (right) {

        // 创建临时变量保存IR的值，以及线性IR指令
        node->blockInsts.addInst(right->blockInsts);

        // 返回值赋值到函数返回值变量上，然后跳转到函数的尾部
        node->blockInsts.addInst(new MoveInstruction(currentFunc, currentFunc->getReturnValue(), right->val));

        node->val = right->val;
    } else {
        // 没有返回值
        node->val = nullptr;
    }

    // 跳转到函数的尾部出口指令上
    node->blockInsts.addInst(new GotoInstruction(currentFunc, static_cast<LabelInstruction*>(currentFunc->getExitLabel())));

    return true;
}

/// @brief 类型叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_type(ast_node * node)
{
    // 不需要做什么，直接从节点中获取即可。

    return true;
}

/// @brief 标识符叶子节点翻译成线性中间IR，变量声明的不走这个语句
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_var_id(ast_node * node)
{
    Value * val;

    // 查找ID型Value
    // 变量，则需要在符号表中查找对应的值

    val = module->findVarValue(node->name);

    node->val = val;

    return true;
}

/// @brief 无符号整数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_uint(ast_node * node)
{
    ConstInt * val;

    // 新建一个整数常量Value
    val = module->newConstInt((int32_t) node->integer_val);

    node->val = val;

    return true;
}

/// @brief 变量声明语句节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_declare_statment(ast_node * node)
{
    bool result = false;

    for (auto & child: node->sons) {

        // 遍历每个变量声明
        result = ir_variable_declare(child);
        if (!result) {
            break;
        }
    }

    return result;
}

/// @brief 变量定声明节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_variable_declare(ast_node * node)
{
    // 共有两个孩子，第一个类型，第二个变量名

    // TODO 这里可强化类型等检查

    node->val = module->newVarValue(node->sons[0]->type, node->sons[1]->name);

    return true;
}

/// @brief 单目负运算符AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_unary_minus(ast_node * node)
{
    ast_node * operand_node = node->sons[0];

    // 先计算操作数
    ast_node * operand = ir_visit_ast_node(operand_node);
    if (!operand) {
        // 操作数计算失败
        return false;
    }

    // 如果操作数是常量，直接计算结果
    if (auto constInt = dynamic_cast<ConstInt *>(operand->val)) {
        int32_t val = constInt->getVal();
        node->val = module->newConstInt(-val);
        return true;
    }

    // 创建一个常量0
    auto zero = module->newConstInt(0);

    // 创建减法指令：0 - operand
    BinaryInstruction * subInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_SUB_I,
                                                        zero,
                                                        operand->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(operand->blockInsts);
    node->blockInsts.addInst(subInst);

    node->val = subInst;

    return true;
}

/// @brief 关系运算符大于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_gt(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建大于指令
    BinaryInstruction * gtInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_GT_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(gtInst);

    node->val = gtInst;

    return true;
}

/// @brief 关系运算符大于等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ge(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建大于等于指令
    BinaryInstruction * geInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_GE_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(geInst);

    node->val = geInst;

    return true;
}

/// @brief 关系运算符小于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_lt(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建小于指令
    BinaryInstruction * ltInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_LT_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(ltInst);

    node->val = ltInst;

    return true;
}

/// @brief 关系运算符小于等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_le(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建小于等于指令
    BinaryInstruction * leInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_LE_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(leInst);

    node->val = leInst;

    return true;
}

/// @brief 关系运算符等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_eq(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建等于指令
    BinaryInstruction * eqInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_EQ_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(eqInst);

    node->val = eqInst;

    return true;
}

/// @brief 关系运算符不等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ne(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建不等于指令
    BinaryInstruction * neInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_NE_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(neInst);

    node->val = neInst;

    return true;
}

/// @brief 逻辑运算符与AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_and(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建逻辑与指令
    BinaryInstruction * andInst = new BinaryInstruction(module->getCurrentFunction(),
                                                       IRInstOperator::IRINST_OP_AND_I,
                                                       left->val,
                                                       right->val,
                                                       IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(andInst);

    node->val = andInst;

    return true;
}

/// @brief 逻辑运算符或AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_or(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 先计算左操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        return false;
    }

    // 再计算右操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        return false;
    }

    // 创建逻辑或指令
    BinaryInstruction * orInst = new BinaryInstruction(module->getCurrentFunction(),
                                                      IRInstOperator::IRINST_OP_OR_I,
                                                      left->val,
                                                      right->val,
                                                      IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(orInst);

    node->val = orInst;

    return true;
}

/// @brief 逻辑运算符非AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_not(ast_node * node)
{
    ast_node * operand_node = node->sons[0];

    // 先计算操作数
    ast_node * operand = ir_visit_ast_node(operand_node);
    if (!operand) {
        return false;
    }

    // 创建一个常量1
    auto one = module->newConstInt(1);

    // 创建逻辑非指令
    BinaryInstruction * notInst = new BinaryInstruction(module->getCurrentFunction(),
                                                       IRInstOperator::IRINST_OP_NOT_I,
                                                       operand->val,
                                                       one,
                                                       IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(operand->blockInsts);
    node->blockInsts.addInst(notInst);

    node->val = notInst;

    return true;
}
bool IRGenerator::ir_if(ast_node * node)
{
    // if语句有两个子节点：条件表达式和then分支
    ast_node * cond_node = node->sons[0];
    ast_node * then_node = node->sons[1];

    // 生成条件表达式的代码
    ast_node * cond_result = ir_visit_ast_node(cond_node);
    if (!cond_result) return false;

    // 创建标签
    LabelInstruction * then_label = new LabelInstruction(module->getCurrentFunction(), "then");
    LabelInstruction * exit_label = new LabelInstruction(module->getCurrentFunction(), "exit");

    // 生成条件跳转指令：如果条件为真，跳转到then标签
    GotoInstruction * cond_jump = new GotoInstruction(module->getCurrentFunction(), then_label, cond_result->val);
    node->blockInsts.addInst(cond_result->blockInsts);
    node->blockInsts.addInst(cond_jump);

    // 生成跳转到出口的指令
    GotoInstruction * exit_jump = new GotoInstruction(module->getCurrentFunction(), exit_label);
    node->blockInsts.addInst(exit_jump);

    // 插入then标签
    node->blockInsts.addInst(then_label);

    // 生成then分支的代码
    ast_node * then_result = ir_visit_ast_node(then_node);
    if (!then_result) return false;
    node->blockInsts.addInst(then_result->blockInsts);

    // 插入出口标签
    node->blockInsts.addInst(exit_label);

    return true;
}

bool IRGenerator::ir_if_else(ast_node * node)
{
    // if-else语句有三个子节点：条件表达式、then分支和else分支
    ast_node * cond_node = node->sons[0];
    ast_node * then_node = node->sons[1];
    ast_node * else_node = node->sons[2];

    // 生成条件表达式的代码
    ast_node * cond_result = ir_visit_ast_node(cond_node);
    if (!cond_result) return false;

    // 创建标签
    LabelInstruction * then_label = new LabelInstruction(module->getCurrentFunction(), "then");
    LabelInstruction * else_label = new LabelInstruction(module->getCurrentFunction(), "else");
    LabelInstruction * exit_label = new LabelInstruction(module->getCurrentFunction(), "exit");

    // 生成条件跳转指令：如果条件为真，跳转到then标签
    GotoInstruction * cond_jump = new GotoInstruction(module->getCurrentFunction(), then_label, cond_result->val);
    node->blockInsts.addInst(cond_result->blockInsts);
    node->blockInsts.addInst(cond_jump);

    // 生成跳转到else标签的指令
    GotoInstruction * else_jump = new GotoInstruction(module->getCurrentFunction(), else_label);
    node->blockInsts.addInst(else_jump);

    // 插入then标签
    node->blockInsts.addInst(then_label);

    // 生成then分支的代码
    ast_node * then_result = ir_visit_ast_node(then_node);
    if (!then_result) return false;
    node->blockInsts.addInst(then_result->blockInsts);

    // 生成跳转到出口的指令
    GotoInstruction * exit_jump = new GotoInstruction(module->getCurrentFunction(), exit_label);
    node->blockInsts.addInst(exit_jump);

    // 插入else标签
    node->blockInsts.addInst(else_label);

    // 生成else分支的代码
    ast_node * else_result = ir_visit_ast_node(else_node);
    if (!else_result) return false;
    node->blockInsts.addInst(else_result->blockInsts);

    // 插入出口标签
    node->blockInsts.addInst(exit_label);

    return true;
}

bool IRGenerator::ir_while(ast_node * node)
{
    // while语句有两个子节点：条件表达式和循环体
    ast_node * cond_node = node->sons[0];
    ast_node * body_node = node->sons[1];

    // 创建标签
    LabelInstruction * cond_label = new LabelInstruction(module->getCurrentFunction(), "while_cond");
    LabelInstruction * body_label = new LabelInstruction(module->getCurrentFunction(), "while_body");
    LabelInstruction * exit_label = new LabelInstruction(module->getCurrentFunction(), "while_exit");

    // 保存当前循环的标签，用于break和continue语句
    LabelInstruction * old_continue_label = currentContinueLabel;
    LabelInstruction * old_break_label = currentBreakLabel;
    currentContinueLabel = cond_label;
    currentBreakLabel = exit_label;

    // 插入条件标签
    node->blockInsts.addInst(cond_label);

    // 生成条件表达式的代码
    ast_node * cond_result = ir_visit_ast_node(cond_node);
    if (!cond_result) return false;
    node->blockInsts.addInst(cond_result->blockInsts);

    // 生成条件跳转指令：如果条件为真，跳转到循环体标签
    GotoInstruction * cond_jump = new GotoInstruction(module->getCurrentFunction(), body_label, cond_result->val);
    node->blockInsts.addInst(cond_jump);

    // 生成跳转到出口的指令
    GotoInstruction * exit_jump = new GotoInstruction(module->getCurrentFunction(), exit_label);
    node->blockInsts.addInst(exit_jump);

    // 插入循环体标签
    node->blockInsts.addInst(body_label);

    // 生成循环体的代码
    ast_node * body_result = ir_visit_ast_node(body_node);
    if (!body_result) return false;
    node->blockInsts.addInst(body_result->blockInsts);

    // 生成跳转回条件的指令
    GotoInstruction * back_jump = new GotoInstruction(module->getCurrentFunction(), cond_label);
    node->blockInsts.addInst(back_jump);

    // 插入出口标签
    node->blockInsts.addInst(exit_label);

    // 恢复之前的break和continue标签
    currentContinueLabel = old_continue_label;
    currentBreakLabel = old_break_label;

    return true;
}

bool IRGenerator::ir_break(ast_node * node)
{
    // 获取当前循环的break标签
    if (!currentBreakLabel) {
        // 如果不在循环中，报错
        std::cerr << "Error: break statement not in loop" << std::endl;
        return false;
    }

    // 生成跳转到break标签的指令
    GotoInstruction * break_jump = new GotoInstruction(module->getCurrentFunction(), currentBreakLabel);
    node->blockInsts.addInst(break_jump);

    return true;
}

bool IRGenerator::ir_continue(ast_node * node)
{
    // 获取当前循环的continue标签
    if (!currentContinueLabel) {
        // 如果不在循环中，报错
        std::cerr << "Error: continue statement not in loop" << std::endl;
        return false;
    }

    // 生成跳转到continue标签的指令
    GotoInstruction * continue_jump = new GotoInstruction(module->getCurrentFunction(), currentContinueLabel);
    node->blockInsts.addInst(continue_jump);

    return true;
}
std::string IRGenerator::generateLabelName(const std::string & prefix)
{
    return prefix + "_" + std::to_string(labelCounter++);
}
