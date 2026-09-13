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

Comment::Comment(std::string message)
: m_message(std::move(message))
{
}

Comment::~Comment() = default;

std::string Comment::message() {
	return m_message;
}
std::string Comment::to_string() {
	return std::format("Comment({})", message());
}
NodeType Comment::type() {
	return NodeType::COMMENT;
}

Assignment::Assignment(std::string variable, std::unique_ptr<Expression> value)
: m_variable(std::move(variable)),
  m_value(std::move(value))
{
}

Assignment::~Assignment() = default;

std::string Assignment::variable() {
	return m_variable;
}

std::unique_ptr<Expression>& Assignment::value() {
	return m_value;
}

std::string Assignment::to_string() {
	return std::format("Assign('{}' = {})", m_variable, m_value->to_string());
}

NodeType Assignment::type() {
	return NodeType::STATEMNET_ASSIGN;
}