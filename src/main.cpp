#include <_drivers/cpp/cpp_driver_config.h>
#include <parser/parser.h>
#include <core/mcc_object.h>
#include <lexer/lexer.h>
#include <pipeline/args.h>
#include <pipeline/compiler.h>

using namespace mcc;
using namespace mcc::pipeline;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;
using namespace mcc::core;
using namespace mcc::lexer;
using namespace mcc::parser;
using namespace mcc::codegen;
using namespace mcc::drivers;
using namespace mcc::drivers::cpp;

int main(int argc, char** argv) {

    // Setup compiler
    CompilerArguments args(argc, argv);
    CompilerPipeline compiler(args.source_lang(), args.dest_lang(), args.source_file());

    // Compile the program
    compiler.compile();
	compiler.write(args.dest_file());

    return 0;
}