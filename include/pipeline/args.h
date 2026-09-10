//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_PIPELINE_ARGS_H
#define MCC_PIPELINE_ARGS_H

#include <pipeline/compiler.h>

namespace mcc::pipeline {

	class CompilerArguments {

		public:
			CompilerArguments(size_t num_args, char** args);
			~CompilerArguments();

			Language source_lang();
			Language dest_lang();

			std::string source_file();
			std::string dest_file();

	};

}

#endif//MCC_PIPELINE_ARGS_H
