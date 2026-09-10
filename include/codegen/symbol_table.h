//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_CODEGEN_SYMBOL_TABLE_H
#define MCC_CODEGEN_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>


namespace mcc::codegen {

	class SymbolTable {

		private:

			std::unordered_map<std::string, size_t> m_symbols = {};

			size_t m_current_offset = 0;
			size_t next_offset();

		public:
			SymbolTable();
			~SymbolTable();

			size_t get_offset(const std::string& symbol);

			void add_symbol(std::string symbol, size_t offset);
			size_t new_symbol(std::string symbol);

			void clear();
	};

}

#endif//MCC_CODEGEN_SYMBOL_TABLE_H
