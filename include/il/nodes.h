//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_INTERNAL_NODES_H
#define MCC_INTERNAL_NODES_H

#include <cstdint>
#include <memory>
#include <vector>
#include <core/mcc_object.h>

namespace mcc::InternalLanguage {

	class Node : public core::MCCObject {

		public:
			Node();
			~Node() override;

			void print_indented_string(size_t indent, const char* string);

			void print();
			virtual void print(size_t indent);
	};


	class Expression : public Node {

		// left + op + right

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
	};

	class Statement : public Node {

	};


	class StatementReturn : public Statement {

		private:
			std::unique_ptr<Expression> m_return_value;

		public:

			StatementReturn(std::unique_ptr<Expression> value);
			~StatementReturn() override;

			[[nodiscard]] std::string to_string() final;
	};

	class Block : public Node {

		private:
			std::vector<std::unique_ptr<Statement>> m_statements;

		public:

			Block(std::vector<std::unique_ptr<Statement>> contents);
			~Block();

			void append(std::unique_ptr<Statement> statement);
			std::vector<std::unique_ptr<Statement>>& statements();

			[[nodiscard]] std::string to_string() final;
			void print(size_t indent) override;
	};

	 class Function : public Node {

		 private:
	 		std::string m_name;
	 		std::unique_ptr<Block> m_body;

		 public:

	 		//@todo return type, params

	 		Function(std::string m_name, std::unique_ptr<Block> body);

	 		std::string name();
	 		std::unique_ptr<Block>& body();

			[[nodiscard]] std::string to_string() final;
			void print(size_t indent) override;

	 };

}

#endif//MCC_INTERNAL_NODES_H
