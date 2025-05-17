 #pragma once

#include "Instruction.h"
#include "IntegerType.h"

///
/// @brief 整数比较指令
///
class ICmpInstruction : public Instruction {
public:
    ///
    /// @brief 比较谓词
    ///
    enum Predicate {
        EQ,  ///< equal
        NE,  ///< not equal
        SGT, ///< signed greater than
        SGE, ///< signed greater or equal
        SLT, ///< signed less than
        SLE  ///< signed less or equal
    };

    ///
    /// @brief 构造函数
    /// @param _func 所属函数
    /// @param _pred 比较谓词
    /// @param _lhs 左操作数
    /// @param _rhs 右操作数
    ///
    ICmpInstruction(Function* _func, Predicate _pred, Value* _lhs, Value* _rhs);

    ///
    /// @brief 转换成字符串
    /// @return 返回指令字符串
    ///
    void toString(std::string& str) override;

    ///
    /// @brief 获取比较谓词
    /// @return 比较谓词
    ///
    [[nodiscard]] Predicate getPredicate() const { return pred; }

    ///
    /// @brief 获取左操作数
    /// @return 左操作数
    ///
    [[nodiscard]] Value* getLHS() const { return lhs; }

    ///
    /// @brief 获取右操作数
    /// @return 右操作数
    ///
    [[nodiscard]] Value* getRHS() const { return rhs; }

private:
    ///
    /// @brief 获取谓词字符串
    /// @param pred 比较谓词
    /// @return 谓词字符串
    ///
    static std::string getPredicateString(Predicate pred);

    Predicate pred; ///< 比较谓词
    Value* lhs;     ///< 左操作数
    Value* rhs;     ///< 右操作数
};