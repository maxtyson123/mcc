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

bool ErrorReporter::has_errors() {
    return !m_errors.empty();
}
const std::vector<CompilerError>& ErrorReporter::errors() {
	return m_errors;
}
void ErrorReporter::print_all() {

	for (auto& error : m_errors)
		std::printf("%s\n", error.to_string().c_str());

}


