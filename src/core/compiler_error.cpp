//
// Created by Max Tyson on 09/09/2026.
//

#include <core/compiler_error.h>

using namespace mcc;
using namespace mcc::core;

CompilerError::CompilerError(Severity severity, SourceLocation location, const std::string& message)
: m_severity(severity),
  m_location(location),
  m_message(message)
{
}

CompilerError::~CompilerError() = default;

Severity CompilerError::severity() {
  return m_severity;
}

std::string CompilerError::to_string() {

  std::string level;
  switch (m_severity) {

    case Severity::ERROR: {
      level = "Error";
      break;
    };

    case Severity::WARNING: {
      level = "Warning";
      break;
    }

  }

  return std::format("{}: {} - {}", level, m_location.to_string(), m_message);
}


