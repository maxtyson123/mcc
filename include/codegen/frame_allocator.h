//
// Created by Max Tyson on 11/09/2026.
//

#ifndef MCC_CODEGEN_FRAME_ALLOCATOR_H
#define MCC_CODEGEN_FRAME_ALLOCATOR_H

#include <il/nodes/structure.h>
#include <codegen/symbol_table.h>

namespace mcc::codegen {

	class FrameAllocator {


		public:

			FrameAllocator();
			~FrameAllocator();

			size_t allocate(InternalLanguage::nodes::Block& block, SymbolTable& symbols);

	};

}

#endif//MCC_CODEGEN_FRAME_ALLOCATOR_H
