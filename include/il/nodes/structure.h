//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_INTERNAL_NODES_STRUCTURE_H
#define MCC_INTERNAL_NODES_STRUCTURE_H

#include <il/nodes/node.h>
#include <il/nodes/statement.h>

namespace mcc::InternalLanguage::nodes {


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

			[[nodiscard]] NodeType type() final;
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

			[[nodiscard]] NodeType type() final;
	};

}

#endif//MCC_INTERNAL_NODES_STRUCTURE_H
