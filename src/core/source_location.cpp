//
// Created by Max Tyson on 09/09/2026.
//

#include <core/source_location.h>

using namespace mcc;
using namespace mcc::core;

SourceLocation::SourceLocation(const std::string& message, size_t line, size_t col)
: m_file(m_file),
  m_line(line),
  m_col(col)
{

}

SourceLocation::~SourceLocation() = default;

std::string SourceLocation::file() {
  return m_file;
}

std::string SourceLocation::to_string() {
  return std::format("{} {}:{}", m_file, m_line, m_col);
}



