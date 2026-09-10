#include <fstream>
#include <iostream>
#include <sstream>

#include <_drivers/cpp/cpp_driver_config.h>
#include <codegen/asm_emitter.h>
#include <parser/parser.h>
#include <core/mcc_object.h>
#include <lexer/lexer.h>

using namespace mcc;
using namespace mcc::InternalLanguage;
using namespace mcc::core;
using namespace mcc::lexer;
using namespace mcc::parser;
using namespace mcc::codegen;
using namespace mcc::drivers;
using namespace mcc::drivers::cpp;

int main() {

    // Read the args
    // CompilerArguments args;
    // args.load(argc, argv)

    // Compile the program
    // CompilerPipeline compiler;
    // compiler.compile(args.source_lang(), args.target_lang(), args.source_code())

	// Open source code
	std::ifstream file("../test/main.cpp");
	if (!file.is_open())
		return 1;

	// Read to string
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	CPPDriverConfig cpp;

	ErrorReporter errors;
	Lexer lexer(content, "main.cpp", cpp.lexer_config(), errors);

	//todo parse as program not function
	Parser parser(lexer, errors);
	std::unique_ptr<Function> prog = parser.parse_function();

	std::printf("Program:\n");
	prog->print(0);

	AsmEmitter codegen;
	std::printf("Assembled Program:\n");
	std::printf("%s", codegen.emit_function(*prog).c_str());


	errors.print_all();
    return 0;
}