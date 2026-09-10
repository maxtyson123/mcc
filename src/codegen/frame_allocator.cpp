//
// Created by Max Tyson on 11/09/2026.
//

#include <codegen/frame_allocator.h>

using namespace mcc;
using namespace mcc::codegen;
using namespace mcc::InternalLanguage::nodes;

FrameAllocator::FrameAllocator() = default;
FrameAllocator::~FrameAllocator() = default;

size_t FrameAllocator::allocate(Block& block, SymbolTable& symbols) {

	size_t space = 0;

	for (auto& statement : block.statements())
		if (statement->type() == NodeType::DECLARATION_VARIABLE) {

			// Reserve space and define the variable
			auto& var_decl = (VariableDeclaration&)(*statement);
			space += symbols.new_symbol(var_decl.name());

		}
	return space;
}