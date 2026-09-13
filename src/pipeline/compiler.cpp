//
// Created by Max Tyson on 10/09/2026.
//

#include <fstream>
#include <pipeline/compiler.h>

#include "parser/parser.h"

#include <sstream>

using namespace mcc;
using namespace mcc::core;
using namespace mcc::pipeline;
using namespace mcc::lexer;
using namespace mcc::parser;
using namespace mcc::drivers::cpp;
using namespace mcc::InternalLanguage::nodes;

CompilerPipeline::CompilerPipeline(Language source_lang, Language dest_lang, std::string source_file)
: m_source_lang(source_lang),
  m_dest_lang(dest_lang),
  m_config(load_config())
{

	// Open source code
	std::ifstream input(source_file);
	if (!input.is_open())
		m_errors.fatal("Failed to open source file");

	// Read to string
	m_source_code << input.rdbuf();

	// Setup stages
	m_lexer = std::make_unique<Lexer>(m_source_code.str(), source_file, m_config.lexer_config(), m_errors);
	m_parser = std::make_unique<Parser>(m_errors);

}

CompilerPipeline::~CompilerPipeline() = default;

driver::DriverConfig CompilerPipeline::load_config() {

	switch (m_source_lang) {

		case Language::CPP:
			return CPPDriverConfig();

	}
}

void CompilerPipeline::validate() {

	// Print any warnings or errors
	printf("\n");
	m_errors.print_all();

	// Cant continue to next stage
	if (m_errors.has_errors())
		m_errors.fatal("Pipeline stage failed with errors");

}

void CompilerPipeline::compile() {

	// Run Lexical analysis
	std::vector<Token> tokens = m_lexer->tokenize_all();
	validate();

	// Print tokens
	std::printf("Tokenized Program:\n");
	for (auto& token : tokens)
		std::printf("%s\n", token.to_string().c_str());

	// Run syntactic analysis
	m_parser->load_tokens(std::make_unique<std::vector<Token>>(tokens));
	std::unique_ptr<Program> prog = m_parser->parse_program();
	validate();

	// Print AST
	std::printf("Parsed Program:\n");
	prog->print(0);

	// Run code generation
	m_compiled_code << m_assembler.emit_program(*prog);
	validate();

	// Print compiled source code
	std::printf("\nAssembled Program:\n");
	std::printf("%s", m_compiled_code.str().c_str());
}

void CompilerPipeline::write(std::string dest_file) {

	//@todo make dest file

	// Open source code
	std::ofstream output(dest_file);
	if (!output.is_open())
		m_errors.fatal("Cannot open output file");

	output << m_compiled_code.str();
}

