//
// Created by Max Tyson on 10/09/2026.
//

#include <format>

#include <il/nodes/expression.h>
#include <core/print_helpers.h>

using namespace mcc;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;


LiteralInteger::LiteralInteger(int64_t value)
: m_value(value)
{
}

LiteralInteger::~LiteralInteger() = default;

int64_t LiteralInteger::value() {
	return m_value;
}

std::string LiteralInteger::to_string() {
	return std::format("{}", m_value);
}

NodeType LiteralInteger::type() {
	return NodeType::LiteralInteger;
}
BinaryOperation::BinaryOperation(std::unique_ptr<Expression> m_left, BinaryOperator m_op, std::unique_ptr<Expression> m_right)
: m_left(std::move(m_left)),
  m_op(m_op),
  m_right(std::move(m_right))
{
}

BinaryOperation::~BinaryOperation() = default;

std::unique_ptr<Expression>& BinaryOperation::left() {
	return m_left;
}

BinaryOperator BinaryOperation::op() {
	return m_op;
}

std::unique_ptr<Expression>& BinaryOperation::right() {
	return m_right;
}

std::string BinaryOperation::to_string() {
	return std::format("({} {} {})", m_left->to_string(), m_op, m_right->to_string());
}
NodeType BinaryOperation::type() {
	return NodeType::BinaryOperation;
}