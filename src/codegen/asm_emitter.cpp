//
// Created by Max Tyson on 10/09/2026.
//

#include <codegen/asm_emitter.h>

#include "il/nodes/control.h"

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
			bool comparison = false;

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

				case BinaryOperator::EQUALITY : {
					op = "sete";
					comparison = true;
					break;
				}

				case BinaryOperator::INEQUALITY : {
					op = "setne";
					comparison = true;
					break;
				}

				case BinaryOperator::LESS_THAN : {
					op = "setl";
					comparison = true;
					break;
				}


				case BinaryOperator::LESS_THAN_EQ : {
					op = "setle";
					comparison = true;
					break;
				}

				case BinaryOperator::MORE_THAN : {
					op = "setm";
					comparison = true;
					break;
				}

				case BinaryOperator::MORE_THAN_EQ : {
					op = "setme";
					comparison = true;
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
			if (!comparison) {
				m_output << std::format("{} rax, rbx\n", op);
				break;
			}

			m_output << std::format("cmp rax, rbx\n", op);
			m_output << std::format("{} al\n", op);
			m_output << "movzx rax, al\n";
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

void AsmEmitter::emit_control_while(WhileLoop& statement) {

	std::string start_label = m_labels.next();
	std::string end_label = m_labels.next();

	// Start
	m_output << std::format("{}:\n", start_label);

	// Jump out of loop if condition is false
	emit_expression(*statement.condition());
	m_output << std::format("cmp rax, 0\n");
	m_output << std::format("je {}\n", end_label);

	// Loop on the body
	emit_statement(*statement.body());
	m_output << std::format("jmp {}\n", start_label);

	// Exit
	m_output << std::format("{}:\n", end_label);
}

void AsmEmitter::emit_control_if_else(IfElseStatement& statement) {

	std::string start_label = m_labels.next();
	std::string else_label = m_labels.next();
	std::string end_label = m_labels.next();

	// Jump to else if condition is false
	emit_expression(*statement.condition());
	m_output << std::format("cmp rax, 0\n");
	m_output << std::format("je {}\n", else_label);

	// True body, skipping false
	emit_statement(*statement.body());
	m_output << std::format("jmp {}\n", end_label);

	// False body
	m_output << std::format("{}:\n", else_label);
	if (statement.has_else())
		emit_statement(*statement.else_body());
	m_output << std::format("jmp {}\n", end_label);

	// Exit
	m_output << std::format("{}:\n", end_label);
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

		case NodeType::BLOCK : {
			emit_block((Block&)statement);
			return;
		}

		case NodeType::LOOP_WHILE :  {
			emit_control_while((WhileLoop&)statement);
			return;
		}

		case NodeType::IF_ELSE : {
			emit_control_if_else((IfElseStatement&)statement);
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