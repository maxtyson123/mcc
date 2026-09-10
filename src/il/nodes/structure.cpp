//
// Created by Max Tyson on 10/09/2026.
//

#include <il/nodes/structure.h>

#include <format>
#include <utility>

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

Declaration::Declaration(std::string name)
: m_name(std::move(name))
{

}

Declaration::~Declaration() = default;

std::string Declaration::name() {
	return m_name;
}

FunctionDeclaration::FunctionDeclaration(std::string name)
: FunctionDeclaration(std::move(name), std::make_unique<Block>(std::vector<std::unique_ptr<Statement>>{}))
{

}

FunctionDeclaration::FunctionDeclaration(std::string name, std::unique_ptr<Block> body)
: Declaration(std::move(name)),
  m_body(std::move(body))
{
}

FunctionDeclaration::~FunctionDeclaration() = default;

std::unique_ptr<Block>& FunctionDeclaration::body() {
	return m_body;
}
std::string FunctionDeclaration::to_string() {
	return std::format("Function({}, {})", name(), m_body->to_string());
}

void FunctionDeclaration::print(size_t indent) {

	print_indented_string(indent, std::format("Function'{}'()", name()).c_str());
	m_body->print(indent + 1);
}

NodeType FunctionDeclaration::type() {
	return NodeType::DECLARATION_FUNCTION;
}

VariableDeclaration::VariableDeclaration(std::string name, std::unique_ptr<Expression> initialiser)
: Declaration(std::move(name)),
  m_initialiser(std::move(initialiser))
{
}

VariableDeclaration::~VariableDeclaration() = default;

std::unique_ptr<Expression>& VariableDeclaration::initialiser() {
	return m_initialiser;
}
std::string VariableDeclaration::to_string() {
	return std::format("Variable({} = {})", name(), m_initialiser ? m_initialiser->to_string() : "null");
}

NodeType VariableDeclaration::type() {
	return NodeType::DECLARATION_VARIABLE;
}

Program::Program() = default;

Program::Program(std::vector<std::unique_ptr<Declaration>> declarations)
: m_declarations(std::move(declarations))
{

}

Program::~Program() = default;

void Program::append(std::unique_ptr<Declaration> declaration) {
	m_declarations.push_back(std::move(declaration));
}

std::vector<std::unique_ptr<Declaration>>& Program::declarations() {
	return m_declarations;
}

std::string Program::to_string() {
	return std::format("Block of {} statements", m_declarations.size());
}

void Program::print(size_t indent) {

	for (auto& declaration : declarations())
		declaration->print(indent);

}

NodeType Program::type() {
	return NodeType::PROGRAM;
}



