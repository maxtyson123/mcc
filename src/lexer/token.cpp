//
// Created by Max Tyson on 10/09/2026.
//

#include <lexer/token.h>
#include <core/print_helpers.h>

using namespace mcc;
using namespace mcc::core;
using namespace mcc::lexer;

Token::Token(TokenKind kind, std::string lexeme, SourceLocation location)
: m_kind(kind),
  m_lexeme(lexeme),
  m_location(location)
{

}
Token::~Token() = default;

TokenKind Token::kind() const {
	return m_kind;
}

std::string Token::lexeme() {
	return m_lexeme;
}

SourceLocation Token::location() {
	return m_location;
}
std::string Token::to_string() {

	return std::format("Token {} ('{}') at {}", m_kind, m_lexeme, m_location.to_string());
}



