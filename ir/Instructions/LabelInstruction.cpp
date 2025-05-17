#include "VoidType.h"
#include "LabelInstruction.h"

// 初始化静态计数器
int LabelInstruction::labelCounter = 0;

///
/// @brief 构造函数
/// @param _func 所属函数
/// @param _name 标签名称（可选）
///
LabelInstruction::LabelInstruction(Function* _func, const std::string& _name)
    : Instruction(_func, IRInstOperator::IRINST_OP_LABEL, VoidType::getType()),
      name(_name.empty() ? ".L" + std::to_string(++labelCounter) : _name) {}

///
/// @brief 转换成字符串
/// @return 返回指令字符串
///
void LabelInstruction::toString(std::string& str) {
    str = name + ":";
}