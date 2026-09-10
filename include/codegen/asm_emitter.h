//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_CODEGEN_ASM_EMITTER_H
#define MCC_CODEGEN_ASM_EMITTER_H

#include <codegen/label_generator.h>
#include <codegen/symbol_table.h>
#include <il/nodes/expression.h>
#include <il/nodes/statement.h>
#include <il/nodes/structure.h>
#include <sstream>

namespace mcc::codegen {

	class AsmEmitter {

		private:

			std::stringstream m_output = {};

			LabelGenerator m_labels;
			std::string m_current_exit_label;

			codgen::SymbolTable m_symbols;

			void emit_expression(InternalLanguage::nodes::Expression& expression);
			void emit_statement(InternalLanguage::nodes::Statement& statement);
			void emit_block(InternalLanguage::nodes::Block& block);
			void emit_function(InternalLanguage::nodes::FunctionDeclaration& function);
			void emit_declaration(InternalLanguage::nodes::Declaration& declaration);

		public:

			AsmEmitter();
			~AsmEmitter();

			std::string emit_function(InternalLanguage::nodes::Program& program);

	};
}

#endif//MCC_CODEGEN_ASM_EMITTER_H
