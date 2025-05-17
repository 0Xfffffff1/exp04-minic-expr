#include "ICmpInstruction.h"
#include "IntegerType.h"

///
/// @brief 构造函数
/// @param _func 所属函数
/// @param _pred 比较谓词
/// @param _lhs 左操作数
/// @param _rhs 右操作数
///
ICmpInstruction::ICmpInstruction(Function* _func, Predicate _pred, 
                               Value* _lhs, Value* _rhs)
    : Instruction(_func, IRInstOperator::IRINST_OP_ICMP, 
                 IntegerType::getTypeBool()),
      pred(_pred), lhs(_lhs), rhs(_rhs)
{
    addOperand(_lhs);
    addOperand(_rhs);
}

///
/// @brief 获取谓词字符串
/// @param pred 比较谓词
/// @return 谓词字符串
///
std::string ICmpInstruction::getPredicateString(Predicate pred) {
    switch (pred) {
        case EQ:  return "eq";
        case NE:  return "ne";
        case SGT: return "sgt";
        case SGE: return "sge";
        case SLT: return "slt";
        case SLE: return "sle";
        default:  return "unknown";
    }
}

///
/// @brief 转换成字符串
/// @return 返回指令字符串
///
void ICmpInstruction::toString(std::string& str) {
    str = getName() + " = icmp " + getPredicateString(pred) + " " +
          lhs->getType()->toString() + " " +
          lhs->getName() + ", " + rhs->getName();
}