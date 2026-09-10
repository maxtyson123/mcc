//
// Created by Max Tyson on 10/09/2026.
//

#include <il/nodes/node.h>

#include <format>

using namespace mcc;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;

Node::Node() = default;
Node::~Node() = default;


void Node::print_indented_string(size_t indent, const char* string) {

	// Print indentation
	for (int i = 0; i < indent; ++i)
		printf("\t");

	printf("%s\n", string);

}

void Node::print() {

	// Start of print chain
	print(0);
}

void Node::print(size_t indent) {
	print_indented_string(indent, to_string().c_str());
}