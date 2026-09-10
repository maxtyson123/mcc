//
// Created by Max Tyson on 10/09/2026.
//

#include <codegen/label_generator.h>
#include <il/nodes.h>
#include <sstream>

namespace mcc::codegen {

	class AsmEmitter {

		private:

			std::stringstream m_output = {};

			LabelGenerator m_labels;
			std::string m_current_exit_label;

			void emit_expression(InternalLanguage::Expression& expression);
			void emit_statement(InternalLanguage::Statement& statement);
			void emit_block(InternalLanguage::Block& block);


		public:

			AsmEmitter();
			~AsmEmitter();

			std::string emit_function(InternalLanguage::Function& function);


	};

}