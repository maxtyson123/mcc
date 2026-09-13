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

	size_t highest_offset = 0;

	for (auto& statement : block.statements()) {

		switch (statement->type()) {

			case NodeType::DECLARATION_VARIABLE : {

				// Reserve space and define the variable
				auto& var_decl = (VariableDeclaration&)(*statement);
				highest_offset = symbols.new_symbol(var_decl.name());

				break;
			}

			case NodeType::BLOCK : {

				// Recurse to allocate any variable definitions within blocks such as control bodys
				highest_offset = allocate((Block&)(*statement), symbols);
				break;
			}
		}
	}


	return highest_offset;
}