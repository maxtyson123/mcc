//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_INTERNAL_NODES_STATEMENT_H
#define MCC_INTERNAL_NODES_STATEMENT_H

#include <il/nodes/node.h>
#include <il/nodes/expression.h>

namespace mcc::InternalLanguage::nodes {


	class Statement : public Node {

	};


	class StatementReturn : public Statement {

		private:
			std::unique_ptr<Expression> m_return_value;

		public:

			StatementReturn(std::unique_ptr<Expression> value);
			~StatementReturn() override;

			[[nodiscard]] std::unique_ptr<Expression>& return_value();

			[[nodiscard]] std::string to_string() final;

			[[nodiscard]] NodeType type() final;
	};

}


#endif//MCC_INTERNAL_NODES_STATEMENT_H
