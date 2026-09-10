//
// Created by Max Tyson on 10/09/2026.
//

#include <pipeline/args.h>

using namespace mcc;
using namespace mcc::pipeline;

CompilerArguments::CompilerArguments(size_t num_args, char** args) {
}

CompilerArguments::~CompilerArguments() = default;

Language CompilerArguments::source_lang() {
	return Language::CPP;
}

Language CompilerArguments::dest_lang() {
	return Language::ASM;
}

std::string CompilerArguments::source_file() {
	return "../test/main.cpp";
}

std::string CompilerArguments::dest_file() {
	return "../test/program.s";
}

