#include "VoidType.h"
#include "GotoInstruction.h"

///
/// @brief 无条件跳转指令的构造函数
/// @param _func 所属函数
/// @param _target 跳转目标
/// @param _condition 跳转条件（可选）
///
GotoInstruction::GotoInstruction(Function * _func, LabelInstruction * _target, Value * _condition)
    : Instruction(_func, IRInstOperator::IRINST_OP_GOTO, VoidType::getType()),
      target(_target), condition(_condition)
{
    if (_condition) {
        addOperand(_condition);
    }
}

/// @brief 转换成字符串
void GotoInstruction::toString(std::string & str)
{
    if (condition) {
        str = "if " + condition->getName() + " goto " + target->getName();
    } else {
        str = "goto " + target->getName();
    }
}