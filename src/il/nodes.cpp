//
// Created by Max Tyson on 10/09/2026.
//

#include <il/nodes.h>

#include <format>

using namespace mcc;
using namespace mcc::InternalLanguage;

Node::Node() = default;
Node::~Node() = default;


void Node::print_indented_string(size_t indent, const char* string) {

	// Print indentation
	for (int i = 0; i < indent; ++i)
		printf("\t");

	printf("%s\n", string);

}

void Node::print() {

	// Start of print chain
	print(0);
}

void Node::print(size_t indent) {
	print_indented_string(indent, to_string().c_str());
}

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
