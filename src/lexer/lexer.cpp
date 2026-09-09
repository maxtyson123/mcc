//
// Created by Max Tyson on 10/09/2026.
//

#include <lexer/lexer.h>

using namespace mcc;
using namespace mcc::lexer;
using namespace mcc::core;

Lexer::Lexer(const std::string& source, const std::string& filename, const LexerConfig& config, const ErrorReporter& reporter)
: m_source_code(source),
  m_location(filename, 0, 0),
  m_config(config),
  m_errors(reporter)
{

};

Lexer::~Lexer() = default;

char Lexer::consume() {
	m_location.increment_col();
	return m_source_code[m_pos++];
}


void Lexer::skip_whitespace() {

	char c;
	while (true) {

		// Peek the next part of the source code
		c = m_source_code[m_pos];

		switch (c) {

			// Linebreak
			case '\r' :
			case '\n' : {
				consume();
				m_location.increment_line();
				break;
			}

			// Whitespace
			case '\t':
			case ' ': {
				consume();
				break;
			}

			// Normal char
			default:
				return;
		}
	}
}

Token Lexer::token_at_current(TokenKind kind, const std::string& lexeme) {
	return {kind, lexeme, m_location};
}

Token Lexer::parse_text_token(char c) {

	// Get whole lexeme
	std::string lexeme = std::string(1, c);
	c = consume();

	// Subsequent text token chars may be a letter, digit, or underscore
	while (std::isalnum(c) || c == '_' ) {
		lexeme.push_back(c);
		c = consume();
	}

	// Keywords take precedence
	if (auto keyword = m_config.keywords.find(lexeme); keyword != m_config.keywords.end())
		return token_at_current(keyword->second, lexeme);

	return token_at_current(TokenKind::IDENTIFIER, lexeme);
}

Token Lexer::parse_digit_token(char c) {

	// Get whole lexeme
	std::string lexeme = std::string(1, c);
	c = consume();

	while (std::isdigit(c)) {
		lexeme.push_back(c);
		c = consume();
	}

	return token_at_current(TokenKind::LITERAL_INTEGER, lexeme);
}
Token Lexer::parse_punct_token(const char c) {

	switch (c) {

		case '{' :
			return token_at_current(TokenKind::OPEN_BRACKET, std::string(1, c));

		case '}' :
			return token_at_current(TokenKind::CLOSE_BRACKET, std::string(1, c));

		case '(' :
			return token_at_current(TokenKind::OPEN_PARENTHESES, std::string(1, c));

		case ')' :
			return token_at_current(TokenKind::CLOSE_PARENTHESES, std::string(1, c));

		case ';' :
			return token_at_current(TokenKind::SEMI_COLON, std::string(1, c));

		default: {
			m_errors.report(Severity::ERROR, m_location, "Unknown token of type punctuation");
			return token_at_current(TokenKind::ERROR, std::string(1, c));
		}

	}
}

Token Lexer::parse_next() {

	// Move to the start of the next token
	skip_whitespace();

	// End of file
	if (at_end())
		return token_at_current(TokenKind::END_OF_FILE, nullptr);

	char c = consume();

	// Text token
	if (std::isalnum(c))
		return parse_text_token(c);

	// Digit
	if (std::isdigit(c))
		return parse_digit_token(c);

	// Punctuation
	if (std::ispunct(c))
		return parse_punct_token(c);

	// Must not be a known token
	m_errors.report(Severity::ERROR, m_location, "Unknown token");
	return token_at_current(TokenKind::ERROR, std::string(1, c));

}

Token Lexer::next() {

	// Dont already have the next token
	if (token_buffer.empty())
		return parse_next();

	Token out = token_buffer.front();
	token_buffer.pop();
	return out;
}

Token Lexer::peek(const size_t offset) {

	// Get the next n tokens until offset is reached
	for (size_t i = 0; i < offset + 1; ++i)
		token_buffer.push(parse_next());

	return token_buffer.back();
}

bool Lexer::at_end() const {
	return  m_source_code.empty() || m_pos == m_source_code.size();
}