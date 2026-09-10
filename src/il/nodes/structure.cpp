//
// Created by Max Tyson on 10/09/2026.
//

#include <il/nodes/structure.h>

#include <format>

using namespace mcc;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;

Block::Block(std::vector<std::unique_ptr<Statement>> contents)
: m_statements(std::move(contents))
{
}

Block::~Block() = default;

void Block::append(std::unique_ptr<Statement> statement) {
	m_statements.push_back(std::move(statement));
}

std::vector<std::unique_ptr<Statement>>& Block::statements() {
	return m_statements;
}

std::string Block::to_string() {
	return std::format("Block of {} statements", m_statements.size());
}

void Block::print(size_t indent) {

	print_indented_string(indent, "Block{");
	for (auto& statement : statements())
		statement->print(indent + 1);
	print_indented_string(indent,"}");
}

NodeType Block::type() {
	return NodeType::BLOCK;
}

Function::Function(std::string name, std::unique_ptr<Block> body)
: m_name(name),
  m_body(std::move(body))
{
}

std::string Function::name() {
	return m_name;
}

std::unique_ptr<Block>& Function::body() {
	return m_body;
}
std::string Function::to_string() {
	return std::format("Function({}, {})", m_name, m_body->to_string());
}

void Function::print(size_t indent) {

	print_indented_string(indent, std::format("Function'{}'()", m_name).c_str());
	m_body->print(indent + 1);
}

NodeType Function::type() {
	return NodeType::FUNCTION;
}
