//
// Created by Max Tyson on 10/09/2026.
//

#include <il/nodes/statement.h>

#include <format>

using namespace mcc;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;

StatementReturn::StatementReturn(std::unique_ptr<Expression> value)
: m_return_value(std::move(value))
{
}

StatementReturn::~StatementReturn() = default;

std::unique_ptr<Expression>& StatementReturn::return_value() {
	return m_return_value;
}

std::string StatementReturn::to_string() {
	return std::format("Return: {}", m_return_value->to_string());

}

NodeType StatementReturn::type() {
	return NodeType::STATEMENT_RETURN;
}