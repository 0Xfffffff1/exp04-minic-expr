#include "VoidType.h"
#include "GotoInstruction.h"

///
/// @brief 跳转指令的构造函数
/// @param _func 所属函数
/// @param _target 跳转目标
/// @param _condition 跳转条件（可选）
/// @param _false_target 条件为假时的跳转目标（可选）
///
GotoInstruction::GotoInstruction(Function* _func, LabelInstruction* _target,
                               Value* _condition, LabelInstruction* _false_target)
    : Instruction(_func, IRInstOperator::IRINST_OP_GOTO, VoidType::getType()),
      target(_target), condition(_condition), false_target(_false_target)
{
    if (_condition) {
        addOperand(_condition);
    }
}

///
/// @brief 转换成字符串
/// @return 返回指令字符串
///
void GotoInstruction::toString(std::string& str) {
    if (condition) {
        if (false_target) {
            str = "br i1 " + condition->getName() + 
                  ", label %" + target->getName() + 
                  ", label %" + false_target->getName();
        } else {
            str = "br i1 " + condition->getName() + 
                  ", label %" + target->getName() + 
                  ", label %next";
        }
    } else {
        str = "br label %" + target->getName();
    }
}