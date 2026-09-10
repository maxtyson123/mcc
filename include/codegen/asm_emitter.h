//
// Created by Max Tyson on 10/09/2026.
//

#include <il/nodes/expression.h>
#include <il/nodes/statement.h>
#include <il/nodes/structure.h>
#include <codegen/label_generator.h>
#include <sstream>

namespace mcc::codegen {

	class AsmEmitter {

		private:

			std::stringstream m_output = {};

			LabelGenerator m_labels;
			std::string m_current_exit_label;

			void emit_expression(InternalLanguage::nodes::Expression& expression);
			void emit_statement(InternalLanguage::nodes::Statement& statement);
			void emit_block(InternalLanguage::nodes::Block& block);

		public:

			AsmEmitter();
			~AsmEmitter();

			std::string emit_function(InternalLanguage::nodes::Function& function);


	};

}