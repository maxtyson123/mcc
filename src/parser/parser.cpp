//
// Created by Max Tyson on 10/09/2026.
//

#include <parser/parser.h>

using namespace mcc;
using namespace mcc::parser;
using namespace mcc::lexer;
using namespace mcc::core;
using namespace mcc::InternalLanguage;

Parser::Parser(const Lexer& lexer, ErrorReporter& error_reporter)
: m_lexer(lexer),
  m_errors(error_reporter),
  m_current_token(m_lexer.next())
{

}

Parser::~Parser() = default;

Token Parser::peek() {
	return m_current_token;
}

Token Parser::advance() {

	Token current = m_current_token;
	m_current_token = m_lexer.next();
	return current;
}

bool Parser::check(TokenType type) {
	return peek().type() == type;
}

Token Parser::expect(TokenType type, const std::string& message) {

	// Token must be of expected type
	if (check(type))
		return advance();

	m_errors.report(Stage::PARSER, Severity::ERROR, peek().location(), message);
	return {TokenType::ERROR, peek().lexeme(), peek().location()};
}

std::unique_ptr<LiteralInteger> Parser::parse_literal_integer() {

	Token value = expect(TokenType::LITERAL_INTEGER, "Expected integer value");

	// Parse the lexeme
	int64_t val = std::stoll(value.lexeme());
	return std::make_unique<LiteralInteger>(val);
}

std::unique_ptr<Expression> Parser::parse_expression() {

	Token next = peek();
	switch (next.type()) {

		case TokenType::LITERAL_INTEGER : {
			return parse_literal_integer();
			break;
		}

		default: {
			m_errors.report(Stage::PARSER, Severity::ERROR, next.location(), "Expected a valid expression");
			advance();
			return nullptr;
		}
	}

}

std::unique_ptr<StatementReturn> Parser::parse_return() {

	// expect: return <expression>
	expect(TokenType::KEYWORD_RETURN, "Expected 'return' keyword");
	std::unique_ptr<Expression> expression = parse_expression();

	if (expression == nullptr)
		return nullptr;

	expect(TokenType::SEMI_COLON, "Expected semi-colon to close return statement");

	return std::make_unique<StatementReturn>(std::move(expression));
}

std::unique_ptr<Statement> Parser::parse_statement() {

	Token next = peek();
	switch (next.type()) {

		case TokenType::KEYWORD_RETURN : {
			return parse_return();
			break;
		}

		default: {
			m_errors.report(Stage::PARSER, Severity::ERROR, next.location(), "Expected a valid statement");
			advance();
			return nullptr;
		}
	}
}

std::unique_ptr<Block> Parser::parse_block() {

	// expect: { <statement>; <statement>; ...; }
	expect(TokenType::OPEN_BRACKET, "Expected open bracket to start block");
	std::vector<std::unique_ptr<Statement>> statements = {};

	// Add all the statments
	Token next = peek();
	while (next.type() != TokenType::CLOSE_BRACKET && next.type() != TokenType::END_OF_FILE) {

		// Get the next valid statement
		if (std::unique_ptr<Statement> statement = parse_statement())
			statements.push_back(std::move(statement));

		next = peek();
	}

	expect(TokenType::CLOSE_BRACKET, "Expected close bracket to close block");
	return std::make_unique<Block>(std::move(statements));
}
std::unique_ptr<Function> Parser::parse_function() {

	//@todo types

	// expect: <type> <identifier> <block>
	Token return_type = expect(TokenType::KEYWORD_INT, "Expected function return type");
	Token function_name = expect(TokenType::IDENTIFIER, "Expected function identifier");

	// @todo parse params
	expect(TokenType::OPEN_PARENTHESES, "Expected opening parentheses");
	expect(TokenType::CLOSE_PARENTHESES, "Expected closing parentheses");

	std::unique_ptr<Block> body = parse_block();

	return std::make_unique<Function>(function_name.lexeme(), std::move(body));
}