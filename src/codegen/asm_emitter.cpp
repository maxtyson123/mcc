//
// Created by Max Tyson on 10/09/2026.
//

#include <codegen/asm_emitter.h>

#include <numbers>

using namespace mcc;
using namespace mcc::codegen;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;

AsmEmitter::AsmEmitter() = default;

AsmEmitter::~AsmEmitter() = default;

void AsmEmitter::emit_expression(Expression& expression) {

	switch (expression.type()) {

		case NodeType::LITERAL_INTEGER : {

			auto node = (LiteralInteger&)expression;

			m_output << std::format("mov rax, {}\n", node.value());
			break;
		}

		case NodeType::BINARY_OPERATION : {

			auto& node = (BinaryOperation&)expression;

			std::string op;
			switch (node.op()) {
				case BinaryOperator::ADD : {
					op = "add";
					break;
				}

				case BinaryOperator::SUBTRACT : {
					op = "sub";
					break;
				}

				//@todo signedness

				case BinaryOperator::MULTIPLY : {
					op = "mul";
					break;
				}

				case BinaryOperator::DIVIDE : {
					op = "div";
					break;
				}
			}

			// Resolve lefthand onto stack
			emit_expression(*node.left());
			m_output << std::format("push rax\n");

			// Resolve righthand into eax
			emit_expression(*node.right());
			m_output << std::format("push rax\n");

			// Get left and right back
			m_output << std::format("pop rbx\n");
			m_output << std::format("pop rax\n");

			// Compute
			m_output << std::format("{} rax, rbx\n", op);
			break;
		}

		case NodeType::VARIABLE_REFERENCE : {

			// Load the variable
			auto& node = (VariableReference&)expression;

			size_t offset = m_symbols.get_offset(node.name());
			bool global = offset == 0;

			if (global)
				m_output << std::format("mov rax, [rel {}]\n",  node.name());
			else
				m_output << std::format("mov rax, [rbp - {}]\n",  offset);

		}
	}
}

void AsmEmitter::emit_statement(Statement& statement) {

	switch (statement.type()) {

		case NodeType::STATEMENT_RETURN : {

			// Place ret value
			auto& node = (StatementReturn&)statement;
			emit_expression(*node.return_value());

			// Return label is stored on function entry
			m_output << std::format("jmp {}\n", m_current_exit_label);
			break;
		}

		case NodeType::DECLARATION_VARIABLE : {
			emit_variable_declaration((VariableDeclaration&)statement);
			return;
		}
	}

}

void AsmEmitter::emit_block(Block& block) {

	// Let each statement emit itself
	for (auto& statement : block.statements())
		emit_statement(*statement);

}

void AsmEmitter::emit_variable_declaration(VariableDeclaration& declaration) {

	// Nothing to set as default value
	auto& node = (VariableDeclaration&)declaration;
	if (!node.initialiser())
		return;

	//@todo handle unknown lables

	// Move value into position
	emit_expression(*node.initialiser());

	size_t offset = m_symbols.get_offset(node.name());
	bool global = offset == 0;

	if (global)
		m_output << std::format("mov  [rel {}], rax\n",  node.name());
	else
		m_output << std::format("mov [rbp - {}], rax\n",  offset);
}

void AsmEmitter::emit_function(FunctionDeclaration& function, bool is_setup_function) {

	// Allocate function return
	m_current_exit_label = m_labels.next();

	// Define function
	m_output << std::format("global {}\n", function.name());
	m_output << std::format("{}:\n", function.name());

	// Setup stack frame for function
	m_output << std::format("push rbp\n");
	m_output << std::format("mov rbp, rsp\n");

	// Make space for local variables
	if (!is_setup_function) {
		m_symbols.clear();
		size_t var_space = m_frame_allocator.allocate(*function.body(), m_symbols);
		m_output << std::format("sub rsp, {}\n", var_space);
	}

	emit_block(*function.body());

	// Restore old stack frame
	m_output << std::format("{}:\n", m_current_exit_label);
	m_output << std::format("mov rsp, rbp\n");
	m_output << std::format("pop rbp\n");
	m_output << std::format("ret\n");

}

void AsmEmitter::emit_declaration(Declaration& declaration) {

	switch (declaration.type()) {

		case NodeType::DECLARATION_FUNCTION : {
			emit_function((FunctionDeclaration&)declaration);
			return;
		}

		case NodeType::DECLARATION_VARIABLE : {
			emit_variable_declaration((VariableDeclaration&)declaration);
			return;
		}
	}

}

std::string AsmEmitter::emit_program(Program& program) {

	FunctionDeclaration setup("_setup");

	// Allocate global variables
	m_output << "section .bss\n";
	for (auto& declaration : program.declarations())
		if (declaration->type() == NodeType::DECLARATION_VARIABLE) {

			auto& node = (VariableDeclaration&)(*declaration);
			setup.body()->append(std::move(declaration));

			m_output << std::format("{}:\n", node.name());
			m_output << "resq 1\n";
		}

	// Setup global variables
	m_output << "section .text\n";
	emit_function(setup, true);

	// Code
	for (auto& declaration : program.declarations())
		if (declaration && declaration->type() == NodeType::DECLARATION_FUNCTION)
			emit_function((FunctionDeclaration&)(*declaration));

	return m_output.str();
}