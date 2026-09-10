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
		LiteralNumber,
		LiteralInteger,
		BinaryOperation,

		STATEMENT,
		STATEMENT_RETURN,

		BLOCK,
		FUNCTION,
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
