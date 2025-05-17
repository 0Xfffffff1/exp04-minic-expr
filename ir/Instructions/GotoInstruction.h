#pragma once

#include <string>
#include "Instruction.h"
#include "LabelInstruction.h"
#include "Function.h"

///
/// @brief 跳转指令
///
class GotoInstruction final : public Instruction {
public:
    ///
    /// @brief 跳转指令的构造函数
    /// @param _func 所属函数
    /// @param _target 跳转目标
    /// @param _condition 跳转条件（可选）
    /// @param _false_target 条件为假时的跳转目标（可选）
    ///
    GotoInstruction(Function* _func, LabelInstruction* _target, 
                   Value* _condition = nullptr,
                   LabelInstruction* _false_target = nullptr);

    ///
    /// @brief 转换成字符串
    /// @return 返回指令字符串
    ///
    void toString(std::string& str) override;

    ///
    /// @brief 获取目标Label指令
    /// @return LabelInstruction*
    ///
    [[nodiscard]] LabelInstruction* getTarget() const { return target; }

    ///
    /// @brief 获取跳转条件
    /// @return Value*
    ///
    [[nodiscard]] Value* getCondition() const { return condition; }

    ///
    /// @brief 获取条件为假时的跳转目标
    /// @return LabelInstruction*
    ///
    [[nodiscard]] LabelInstruction* getFalseTarget() const { return false_target; }

private:
    ///
    /// @brief 跳转到的目标Label指令
    ///
    LabelInstruction* target;

    ///
    /// @brief 跳转条件
    ///
    Value* condition;

    ///
    /// @brief 条件为假时的跳转目标
    ///
    LabelInstruction* false_target;
};