//
// Created by Max Tyson on 09/09/2026.
//

#include <core/error_reporter.h>

using namespace mcc;
using namespace mcc::core;

ErrorReporter::ErrorReporter() = default;
ErrorReporter::~ErrorReporter() = default;

void ErrorReporter::report(Stage stage, Severity severity, const SourceLocation& location, const std::string& message) {
    m_errors.emplace_back(stage, severity, location, message);
}
void ErrorReporter::fatal(const std::string& message) {

	printf("\n\nCompilation cannot continue due to FATAL compiler error: %s\n\n", message.c_str());
	exit(1);

}

bool ErrorReporter::has_errors() {

	//@todo prob better way of doing things
	for (auto& error : m_errors)
		if (error.severity() == Severity::ERROR)
			return true;

	return false;
}
const std::vector<CompilerError>& ErrorReporter::errors() {
	return m_errors;
}
void ErrorReporter::print_all() {

	for (auto& error : m_errors)
		std::printf("%s\n", error.to_string().c_str());

}


