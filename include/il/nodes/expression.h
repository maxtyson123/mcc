//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_INTERNAL_NODES_EXPRESSION_H
#define MCC_INTERNAL_NODES_EXPRESSION_H

#include <il/nodes/node.h>

namespace mcc::InternalLanguage::nodes {


	class Expression : public Node {

	};

	class LiteralNumber : public Expression {

	};

	class LiteralInteger : public LiteralNumber {

		private:
			int64_t m_value;

		public:

			LiteralInteger(int64_t value);
			~LiteralInteger() override;

			[[nodiscard]] int64_t value();

			[[nodiscard]] std::string to_string() final;

			[[nodiscard]] NodeType type() final;
	};

	enum class BinaryOperator {

		ADD,
		SUBTRACT,

	};

	class BinaryOperation : public Expression {

		private:

			std::unique_ptr<Expression> m_left;
			BinaryOperator m_op;
			std::unique_ptr<Expression> m_right;

		public:

			BinaryOperation(std::unique_ptr<Expression> m_left, BinaryOperator m_op, std::unique_ptr<Expression> m_right);
			~BinaryOperation();

			[[nodiscard]] std::unique_ptr<Expression>& left();
			[[nodiscard]] BinaryOperator op();
			[[nodiscard]] std::unique_ptr<Expression>& right();

			[[nodiscard]] std::string to_string() final;

			[[nodiscard]] NodeType type() final;

	};

}

#endif//MCC_INTERNAL_NODES_EXPRESSION_H
