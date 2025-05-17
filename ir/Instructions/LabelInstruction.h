#pragma once

#include <string>

#include "Instruction.h"

class Function;

///
/// @brief Label指令
///
class LabelInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    /// @param _func 所属函数
    /// @param _name 标签名称（可选）
    ///
    explicit LabelInstruction(Function * _func, const std::string & _name = "");

    ///
    /// @brief 转换成字符串
    /// @param str 返回指令字符串
    ///
    void toString(std::string & str) override;

    ///
    /// @brief 获取标签名称
    /// @return 标签名称
    ///
    [[nodiscard]] std::string getName() const override { return name; }

    ///
    /// @brief 获取标签名称引用
    /// @return 标签名称引用
    ///
    [[nodiscard]] const std::string & getLabelName() const { return name; }

private:
    ///
    /// @brief 标签名称
    ///
    std::string name;
};