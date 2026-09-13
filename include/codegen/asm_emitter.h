//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_CODEGEN_ASM_EMITTER_H
#define MCC_CODEGEN_ASM_EMITTER_H

#include <codegen/frame_allocator.h>
#include <codegen/label_generator.h>
#include <codegen/symbol_table.h>
#include <il/nodes/expression.h>
#include <il/nodes/statement.h>
#include <il/nodes/structure.h>
#include <il/nodes/control.h>
#include <sstream>

namespace mcc::codegen {

	class AsmEmitter {

		private:

			std::stringstream m_output = {};

			LabelGenerator m_labels;
			std::string m_current_exit_label;

			FrameAllocator m_frame_allocator;
			SymbolTable m_symbols;

			void emit_expression(InternalLanguage::nodes::Expression& expression);
		    void emit_control_while(InternalLanguage::nodes::WhileLoop& statement);
		    void emit_control_if_else(InternalLanguage::nodes::IfElseStatement& statement);
		    void emit_statement(InternalLanguage::nodes::Statement& statement);
			void emit_block(InternalLanguage::nodes::Block& block);
			void emit_function(InternalLanguage::nodes::FunctionDeclaration& function, bool is_setup_function = false);
			void emit_variable_declaration(InternalLanguage::nodes::VariableDeclaration& declaration);
			void emit_declaration(InternalLanguage::nodes::Declaration& declaration);

		public:

			AsmEmitter();
			~AsmEmitter();

			std::string emit_program(InternalLanguage::nodes::Program& program);

	};
}

#endif//MCC_CODEGEN_ASM_EMITTER_H
