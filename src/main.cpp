#include "_drivers/cpp/cpp_driver_config.h"


#include <core/mcc_object.h>
#include <fstream>
#include <lexer/lexer.h>


#include <iostream>
#include <sstream>

using namespace mcc;
using namespace mcc::core;
using namespace mcc::lexer;
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

	Token out = lexer.next();
	while (true) {
		std::printf("Token: %s\n", out.to_string().c_str());

		if (out.kind() == TokenKind::END_OF_FILE)
			break;

		out = lexer.next();
	}

	errors.print_all();
    return 0;
}