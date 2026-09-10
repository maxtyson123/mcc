//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_CODEGEN_LABEL_GENERATOR_H
#define MCC_CODEGEN_LABEL_GENERATOR_H

#include <stddef.h>
#include <string>
#include <format>

namespace mcc::codegen {

	class LabelGenerator {

		private:

			size_t m_next_label = 0;

		public:

			LabelGenerator();
			~LabelGenerator();

			std::string next();
	};

}// namespace mcc::codegen

#endif//MCC_CODEGEN_LABEL_GENERATOR_H
