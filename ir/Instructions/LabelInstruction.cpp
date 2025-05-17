///
/// @file LabelInstruction.cpp
/// @brief Label指令
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#include "VoidType.h"
#include "LabelInstruction.h"

///
/// @brief 构造函数
/// @param _func 所属函数
/// @param _name 标签名称（可选）
///
LabelInstruction::LabelInstruction(Function * _func, const std::string & _name)
    : Instruction(_func, IRInstOperator::IRINST_OP_LABEL, VoidType::getType()), name(_name)
{}

/// @brief 转换成字符串
/// @param str 返回指令字符串
void LabelInstruction::toString(std::string & str)
{
    if (!name.empty()) {
        str = name + ":";
    } else {
        str = IRName + ":";
    }
}
