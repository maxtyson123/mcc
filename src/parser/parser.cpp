//
// Created by Max Tyson on 10/09/2026.
//

#include <parser/parser.h>

using namespace mcc;
using namespace mcc::parser;
using namespace mcc::lexer;
using namespace mcc::core;
using namespace mcc::InternalLanguage;
using namespace mcc::InternalLanguage::nodes;

Parser::Parser(ErrorReporter& error_reporter)
: m_errors(error_reporter)
{

}

Parser::~Parser() = default;

void Parser::load_tokens(std::unique_ptr<std::vector<Token>> tokens) {
	m_tokens = std::move(tokens);
	m_current_token = m_tokens->begin();
}

Token Parser::peek() {
	return *m_current_token;
}

Token Parser::advance() {

	Token current = *(m_current_token++);
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

std::unique_ptr<VariableReference> Parser::parse_variable_reference() {

	Token value = expect(TokenType::IDENTIFIER, "Expected variable identifier");

	// Parse the lexeme
	return std::make_unique<VariableReference>(value.lexeme());

}

std::unique_ptr<Expression> Parser::parse_value_expression() {

	Token next = peek();
	switch (next.type()) {

		case TokenType::LITERAL_INTEGER : {
			return parse_literal_integer();
		}

		case TokenType::IDENTIFIER : {
			return parse_variable_reference();
		}

		default: {
			m_errors.report(Stage::PARSER, Severity::ERROR, next.location(), "Expected a valid expression");
			advance();
			return nullptr;
		}
	}

}

//@todo clean up

std::unique_ptr<Expression> Parser::parse_composite_expression_higher() {

	//NTS: works by grabbing literals as left and right

	std::unique_ptr<Expression> left = parse_value_expression();

	// Keep building until run out of operators
	while (true) {

		BinaryOperator op;

		switch (peek().type()) {
			case TokenType::MULTIPLY : {
				op = BinaryOperator::MULTIPLY;
				break;
			}

			case TokenType::DIVIDE : {
				op = BinaryOperator::DIVIDE;
				break;
			}

			// No more operators to combine right hand expressions
			default: {
				return left;
			}
		}

		// Consume the operator
		advance();

		// Left is now chained with the
		std::unique_ptr<Expression> right = parse_value_expression();
		left = std::make_unique<BinaryOperation>(std::move(left), op, std::move(right));
	}
}

std::unique_ptr<Expression> Parser::parse_composite_expression_lower() {

	//NTS: works by try to grab already made '(x M/D y)' as left and right

	std::unique_ptr<Expression> left = parse_composite_expression_higher();

	// Keep building until run out of operators
	while (true) {

		BinaryOperator op;

		switch (peek().type()) {
			case TokenType::ADD : {
				op = BinaryOperator::ADD;
				break;
			}

			case TokenType::SUBTRACT : {
				op = BinaryOperator::SUBTRACT;
				break;
			}

			// No more operators to combine right hand expressions
			default: {
				return left;
			}
		}

		// Consume the operator
		advance();

		// Left is now chained with the
		std::unique_ptr<Expression> right = parse_composite_expression_higher();
		left = std::make_unique<BinaryOperation>(std::move(left), op, std::move(right));
	}
}
std::unique_ptr<Expression> Parser::parse_expression() {
	return parse_composite_expression_lower();
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

		case TokenType::KEYWORD_INT : {

			//@todo DRY
			Token type_token = expect(TokenType::KEYWORD_INT, "Expected declaration type");
			Token identifier_token = expect(TokenType::IDENTIFIER, "Expected declaration identifier");

			if (type_token.type() == TokenType::ERROR || identifier_token.type() == TokenType::ERROR)
				return nullptr;

			return parse_variable_declaration(type_token, identifier_token);
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

	// Add all the statements
	while (!check(TokenType::CLOSE_BRACKET) && !check(TokenType::END_OF_FILE)) {

		// Get the next valid statement
		if (std::unique_ptr<Statement> statement = parse_statement())
			statements.push_back(std::move(statement));

	}

	expect(TokenType::CLOSE_BRACKET, "Expected close bracket to close block");
	return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<FunctionDeclaration> Parser::parse_function_declaration(Token type_token, Token name_token) {

	// @todo parse params
	expect(TokenType::OPEN_PARENTHESES, "Expected opening parentheses");
	expect(TokenType::CLOSE_PARENTHESES, "Expected closing parentheses");

	std::unique_ptr<Block> body = parse_block();

	return std::make_unique<FunctionDeclaration>(name_token.lexeme(), std::move(body));
}

std::unique_ptr<VariableDeclaration> Parser::parse_variable_declaration(Token type_token, Token name_token) {

	std::unique_ptr<Expression> initialiser = nullptr;

	// Has initialiser
	if (check(TokenType::EQUALS)) {
		advance();
		initialiser = parse_expression();
	}

	Token semicolon = expect(TokenType::SEMI_COLON, "Expected semi colon at end of variable declaration");
	if (semicolon.type() == TokenType::ERROR)
		return nullptr;

	return std::make_unique<VariableDeclaration>(name_token.lexeme(), std::move(initialiser));
}

std::unique_ptr<Declaration> Parser::parse_declaration() {

	//@todo types

	// expect: <type> <identifier> -declaration-specific-
	Token type_token = expect(TokenType::KEYWORD_INT, "Expected declaration type");
	Token identifier_token = expect(TokenType::IDENTIFIER, "Expected declaration identifier");

	// Failed to get declartion setup (@todo should return after each to not break shit)
	if (type_token.type() == TokenType::ERROR || identifier_token.type() == TokenType::ERROR )
		return nullptr;

	switch (peek().type()) {

		case TokenType::OPEN_PARENTHESES:
			return parse_function_declaration(type_token, identifier_token);

		case TokenType::EQUALS:
		case TokenType::SEMI_COLON:
			return parse_variable_declaration(type_token, identifier_token);

		default: {
			m_errors.report(Stage::LEXER, Severity::ERROR, identifier_token.location(), "Expected declaration body");
			return nullptr;
		}
	}
}

std::unique_ptr<Program> Parser::parse_program() {

	// <declaration>; <declaration>; ...;
	std::vector<std::unique_ptr<Declaration>> declarations = {};

	// Add all the statements
	while (!check(TokenType::END_OF_FILE)) {

		// Get the next valid statement
		if (std::unique_ptr<Declaration> statement = parse_declaration())
			declarations.push_back(std::move(statement));

	}

	return std::make_unique<Program>(std::move(declarations));
}