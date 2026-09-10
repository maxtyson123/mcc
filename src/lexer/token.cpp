//
// Created by Max Tyson on 10/09/2026.
//

#include <lexer/token.h>
#include <core/print_helpers.h>

using namespace mcc;
using namespace mcc::core;
using namespace mcc::lexer;

Token::Token(TokenType type, std::string lexeme, SourceLocation location)
: m_type(type),
  m_lexeme(lexeme),
  m_location(location)
{

}
Token::~Token() = default;

TokenType Token::type() const {
	return m_type;
}

std::string Token::lexeme() {
	return m_lexeme;
}

SourceLocation Token::location() {
	return m_location;
}
std::string Token::to_string() {

	return std::format("Token {} ('{}') at {}", m_type, m_lexeme, m_location.to_string());
}



