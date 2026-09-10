//
// Created by Max Tyson on 09/09/2026.
//

#include <core/compiler_error.h>
#include <core/print_helpers.h>

using namespace mcc;
using namespace mcc::core;

CompilerError::CompilerError(Stage stage, Severity severity, SourceLocation location, const std::string& message)
: m_stage(stage),
  m_severity(severity),
  m_location(location),
  m_message(message)
{
}

CompilerError::~CompilerError() = default;

Severity CompilerError::severity() const {
  return m_severity;
}

std::string CompilerError::to_string() {
  return std::format("{} {}: {} - {}", m_stage, m_severity, m_location.to_string(), m_message);
}


