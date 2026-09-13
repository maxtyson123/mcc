//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_INTERNAL_NODES_NODE_H
#define MCC_INTERNAL_NODES_NODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <core/mcc_object.h>

namespace mcc::InternalLanguage::nodes {

	enum class NodeType {

		UNKNOWN,

		EXPRESSION,
		LITERAL_NUMBER,
		LITERAL_INTEGER,
		BINARY_OPERATION,
		VARIABLE_REFERENCE,

		STATEMENT,
		STATEMENT_RETURN,
		COMMENT,

		IF_ELSE,
		LOOP_WHILE,

		BLOCK,

		DECLARATION,
		DECLARATION_FUNCTION,
		DECLARATION_VARIABLE,

		PROGRAM,
	};

	class Node : public core::MCCObject {

		public:
			Node();
			~Node() override;

			void print_indented_string(size_t indent, const char* string);

			void print();
			virtual void print(size_t indent);

			virtual NodeType type() = 0;
	};

}

#endif//MCC_INTERNAL_NODES_H
