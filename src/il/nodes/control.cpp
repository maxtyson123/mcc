//
// Created by Max Tyson on 12/09/2026.
//

#include <il/nodes/control.h>

using namespace mcc;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;

ControlFlow::ControlFlow(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
: m_comparison(std::move(condition)),
  m_body(std::move(body))
{
}

ControlFlow::~ControlFlow() = default;

std::unique_ptr<Expression>& ControlFlow::condition() {
	return m_comparison;
}

std::unique_ptr<Statement>& ControlFlow::body() {
	return m_body;
}

IfElseStatement::IfElseStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body, std::unique_ptr<Statement> else_body)
: ControlFlow(std::move(condition), std::move(body)),
  m_else_body(std::move(else_body))
{
}

IfElseStatement::~IfElseStatement() = default;

bool IfElseStatement::has_else() {
	return m_else_body != nullptr;
}

std::unique_ptr<Statement>& IfElseStatement::else_body() {
	return m_else_body;
}


std::string IfElseStatement::to_string() {

	std::string else_str = has_else() ? std::format(", else -> {}", else_body()->to_string()) : "";

	return std::format("If ({}) -> {} {}", condition()->to_string(), body()->to_string(), else_str);
}

void IfElseStatement::print(size_t indent) {

	print_indented_string(indent, std::format("If({})", condition()->to_string()).c_str());
	body()->print(indent + 1);

	if (!has_else())
		return;

	print_indented_string(indent, "Else");
	else_body()->print(indent + 1);
}

NodeType IfElseStatement::type() {
	return NodeType::IF_ELSE;
}

WhileLoop::WhileLoop(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
: ControlFlow(std::move(condition), std::move(body))
{
}

WhileLoop::~WhileLoop() = default;


std::string WhileLoop::to_string() {

	return std::format("While ({}) -> {}", condition()->to_string(), body()->to_string());
}

void WhileLoop::print(size_t indent) {

	print_indented_string(indent, std::format("While({})", condition()->to_string()).c_str());
	condition()->print(indent + 1);
}

NodeType WhileLoop::type() {
	return NodeType::LOOP_WHILE;
}