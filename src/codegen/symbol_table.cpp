//
// Created by Max Tyson on 10/09/2026.
//

#include <codegen/symbol_table.h>

using namespace mcc;
using namespace mcc::codgen;

SymbolTable::SymbolTable() = default;

SymbolTable::~SymbolTable() = default;

size_t SymbolTable::next_offset() {

	// @todo variable offsets
	size_t offset = m_current_offset;
	m_current_offset += 8;

	return offset;

}

size_t SymbolTable::get_offset(std::string symbol) {

	if (auto offset = m_symbols.find(symbol); offset != m_symbols.end())
		return offset->second;

	return 0;
}

void SymbolTable::add_symbol(std::string symbol, size_t offset) {
	m_symbols[symbol] = offset;
}
size_t SymbolTable::new_symbol(std::string symbol) {

	// Store reference
	size_t offset = next_offset();
	add_symbol(symbol, offset);

	return offset;
}

