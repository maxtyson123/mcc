//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_PIPELINE_COMPILER_H
#define MCC_PIPELINE_COMPILER_H

#include <sstream>

#include <parser/parser.h>
#include <codegen/asm_emitter.h>
#include <core/error_reporter.h>
#include <driver/driver_config.h>
#include <_drivers/cpp/cpp_driver_config.h>

namespace mcc::pipeline {

	enum class Language {

		C,
		CPP,
		ASM,

	};

	class CompilerPipeline {

		private:

			Language m_source_lang;
			Language m_dest_lang;

			core::ErrorReporter m_errors;
			driver::DriverConfig m_config;

			std::stringstream m_source_code = {};
			std::stringstream m_compiled_code = {};

			std::unique_ptr<lexer::Lexer> m_lexer;
			std::unique_ptr<parser::Parser> m_parser;
			codegen::AsmEmitter m_assembler;

			driver::DriverConfig load_config();

		public:
			CompilerPipeline(Language source_lang, Language dest_lang, std::string source_file);
			~CompilerPipeline();

			void validate();

			void compile();
			void write(std::string dest_file);

	};

}

#endif//MCC_PIPELINE_COMPILER_H
