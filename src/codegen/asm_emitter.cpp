//
// Created by Max Tyson on 10/09/2026.
//

#include <codegen/asm_emitter.h>

#include <numbers>

using namespace mcc;
using namespace mcc::codegen;
using namespace mcc::InternalLanguage;

AsmEmitter::AsmEmitter() = default;

AsmEmitter::~AsmEmitter() = default;

void AsmEmitter::emit_expression(Expression& expression) {

	switch (expression.type()) {

		case NodeType::LiteralInteger : {

			auto node = (LiteralInteger&)expression;

			m_output << std::format("mov eax, {}\n", node.value());
			break;
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
	}

}

void AsmEmitter::emit_block(Block& block) {

	// Let each statement emit itself
	for (auto& statement : block.statements())
		emit_statement(*statement);

}

std::string AsmEmitter::emit_function(Function& function) {

	// Allocate function return
	m_current_exit_label = m_labels.next();

	// Define function
	m_output << std::format("global {}\n", function.name());
	m_output << std::format("{}:\n", function.name());

	// Setup stack frame for function
	m_output << std::format("push rbp\n", function.name());
	m_output << std::format("mov rbp, rsp\n", function.name());

	emit_block(*function.body());

	// Restore old stack frame
	m_output << std::format("{}:\n", m_current_exit_label);
	m_output << std::format("mov rsp, rbp\n");
	m_output << std::format("pop rbp\n");
	m_output << std::format("ret\n");

	return m_output.str();
}