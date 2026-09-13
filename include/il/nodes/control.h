//
// Created by Max Tyson on 12/09/2026.
//

#ifndef MCC_INTERNAL_NODES_CONTROL_H
#define MCC_INTERNAL_NODES_CONTROL_H

#include <il/nodes/statement.h>
#include <il/nodes/structure.h>
#include <format>


namespace mcc::InternalLanguage::nodes {

	class ControlFlow : public Statement {

		private:

			std::unique_ptr<Expression> m_comparison;
			std::unique_ptr<Statement> m_body;

		public:
			ControlFlow(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body);
			~ControlFlow();

			std::unique_ptr<Expression>& condition();
			std::unique_ptr<Statement>& body();

	};

	class IfElseStatement : public ControlFlow {

		private:

			std::unique_ptr<Statement> m_else_body;

		public:
			IfElseStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body, std::unique_ptr<Statement> else_body);
			~IfElseStatement();

			bool has_else();
			std::unique_ptr<Statement>& else_body();

			[[nodiscard]] std::string to_string() final;
			void print(size_t indent) override;

			[[nodiscard]] NodeType type() final;

	};

	class WhileLoop : public ControlFlow {

		private:

			std::unique_ptr<Expression> m_comparison;
			std::unique_ptr<Block> m_body;

		public:
			WhileLoop(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body);
			~WhileLoop();

			[[nodiscard]] std::string to_string() final;
			void print(size_t indent) override;

			[[nodiscard]] NodeType type() final;
	};



}

#endif//MCC_INTERNAL_NODES_CONTROL_H
