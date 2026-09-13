//
// Created by Max Tyson on 10/09/2026.
//

#include <lexer/lexer.h>

using namespace mcc;
using namespace mcc::lexer;
using namespace mcc::core;

Lexer::Lexer(const std::string& source, const std::string& filename, LexerConfig  config, ErrorReporter& reporter)
: m_source_code(source),
  m_location(filename, 1, 0),
  m_config(std::move(config)),
  m_errors(reporter)
{

};

Lexer::~Lexer() = default;

char Lexer::lookahead() {
	return m_source_code[m_pos];
}

char Lexer::consume() {
	m_location.increment_col();
	return m_source_code[m_pos++];
}


void Lexer::skip_whitespace() {

	char c;
	while (true) {

		// Peek the next part of the source code
		c = lookahead();

		switch (c) {

			// Linebreak
			case '\r' :
			case '\n' : {
				consume();
				m_location.increment_line();
				break;
			}

			// Whitespace
			case '\t': {
				m_location.increment_col();
				m_location.increment_col();
				m_location.increment_col();

				// fall thru to handle the char itself
			}

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

Token Lexer::token_at_current(TokenType type, const std::string& lexeme) {
	return {type, lexeme, m_location};
}

Token Lexer::parse_text_token(char c) {

	// Get whole lexeme
	std::string lexeme = std::string(1, c);
	c = lookahead();

	// Subsequent text token chars may be a letter, digit, or underscore
	while (std::isalnum(c) || c == '_' ) {
		c = consume();
		lexeme.push_back(c);

		// Cannot consume until known that it belongs to this token
		c = lookahead();
	}

	// Keywords take precedence
	if (auto keyword = m_config.keywords.find(lexeme); keyword != m_config.keywords.end())
		return token_at_current(keyword->second, lexeme);

	return token_at_current(TokenType::IDENTIFIER, lexeme);
}

Token Lexer::parse_digit_token(char c) {

	// Get whole lexeme
	std::string lexeme = std::string(1, c);
	while (std::isdigit(lookahead())) {
		c = consume();
		lexeme.push_back(c);
	}

	return token_at_current(TokenType::LITERAL_INTEGER, lexeme);
}

Token Lexer::parse_comparison_token(char c) {

	// Get whole lexeme
	std::string lexeme = std::string(1, c);
	char nextc = lookahead();

	// Get the equality if it has one
	bool equality = nextc == '=';
	if (equality) {
		lexeme.push_back(nextc);
		consume();
	}

	// Base equality require second equals char
	if (!equality)
		if (c == '=' || c == '!') {
			m_errors.report(Stage::LEXER, Severity::ERROR, m_location, "Malformed comparison token");
			return token_at_current(TokenType::ERROR, lexeme);
		}


	switch (c) {

		case '!' :
			return token_at_current(TokenType::INEQUALITY, lexeme);

		case '=' :
			return token_at_current(TokenType::EQUALITY, lexeme);

		case '<':
			return token_at_current(equality ? TokenType::LESS_THAN_EQ : TokenType::LESS_THAN, lexeme);

		case '>':
			return token_at_current(equality ? TokenType::MORE_THAN_EQ : TokenType::MORE_THAN, lexeme);

	}

}

Token Lexer::parse_comment_token(char c) {

	// @todo multiline comment and inline

	// Consume the comment starter
	std::string lexeme = "/";
	char type = consume();
	lexeme.push_back(type);

	// Get whole lexeme
	while (lookahead() != '\n') {
		c = consume();
		lexeme.push_back(c);
	}

	return token_at_current(TokenType::COMMENT_LINE, lexeme);
}


Token Lexer::parse_punct_token(const char c) {

	switch (c) {

		case '{' :
			return token_at_current(TokenType::OPEN_BRACKET, std::string(1, c));

		case '}' :
			return token_at_current(TokenType::CLOSE_BRACKET, std::string(1, c));

		case '(' :
			return token_at_current(TokenType::OPEN_PARENTHESES, std::string(1, c));

		case ')' :
			return token_at_current(TokenType::CLOSE_PARENTHESES, std::string(1, c));

		case ';' :
			return token_at_current(TokenType::SEMI_COLON, std::string(1, c));

		case '+' :
			return token_at_current(TokenType::ADD, std::string(1, c));

		case '-' :
			return token_at_current(TokenType::SUBTRACT, std::string(1, c));

		case '*' :
			return token_at_current(TokenType::MULTIPLY, std::string(1, c));

		case '=' :
			if (lookahead() == '=')
				return parse_comparison_token(c);

			return token_at_current(TokenType::ASSIGN, std::string(1, c));

		case '<' :
		case '>' :
		case '!' :
			return parse_comparison_token(c);

		case '/' : {
			if (lookahead() == '/')
				return parse_comment_token(c);

			return token_at_current(TokenType::DIVIDE, std::string(1, c));
		}

		default: {
			m_errors.report(Stage::LEXER, Severity::ERROR, m_location, "Unknown token of type punctuation");
			return token_at_current(TokenType::ERROR, std::string(1, c));
		}

	}
}

Token Lexer::parse_next() {

	// Move to the start of the next token
	skip_whitespace();

	// End of file
	if (at_end())
		return token_at_current(TokenType::END_OF_FILE, "EOF");

	char c = consume();

	// Text token
	if (std::isalpha(c) || c == '_')
		return parse_text_token(c);

	//@todo signedness
	// Digit
	if (std::isdigit(c))
		return parse_digit_token(c);

	// Punctuation
	if (std::ispunct(c))
		return parse_punct_token(c);

	// Must not be a known token
	m_errors.report(Stage::LEXER, Severity::ERROR, m_location, "Unknown token");
	return token_at_current(TokenType::ERROR, std::string(1, c));

}

Token Lexer::next() {

	// Dont already have the next token
	if (token_buffer.empty())
		return parse_next();

	Token out = token_buffer.front();
	token_buffer.pop_front();
	return out;
}

Token Lexer::peek(const size_t offset) {

	// Get the next n tokens until offset is reached
	while (token_buffer.size() <= offset)
		token_buffer.push_back(parse_next());

	return token_buffer.back();
}

bool Lexer::at_end() const {
	return  m_source_code.empty() || m_pos == m_source_code.size();
}

std::vector<Token> Lexer::tokenize_all() {

	std::vector<Token> out = {};

	Token token = next();
	while (true) {
		out.push_back(token);

		// No more code to tokenize
		if (token.type() == TokenType::END_OF_FILE)
			break;

		token = next();
	}

	return out;
}