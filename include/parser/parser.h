//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_PARSER_PARSER_H
#define MCC_PARSER_PARSER_H


#include <il/nodes/expression.h>
#include <il/nodes/statement.h>
#include <il/nodes/structure.h>
#include <core/error_reporter.h>
#include <lexer/lexer.h>
#include <lexer/token.h>

namespace mcc::parser {

	class Parser {

		private:

			lexer::Lexer m_lexer;
			core::ErrorReporter& m_errors;

			lexer::Token m_current_token;

			lexer::Token peek();
			lexer::Token advance();

			[[nodiscard]] bool check(lexer::TokenType type);
			lexer::Token expect(lexer::TokenType type, const std::string& message);

		public:
			Parser(const lexer::Lexer& lexer, core::ErrorReporter& error_reporter);
			~Parser();

			std::unique_ptr<InternalLanguage::nodes::LiteralInteger> parse_literal_integer();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_expression();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_composite_expression();

			std::unique_ptr<InternalLanguage::nodes::StatementReturn> parse_return();
			std::unique_ptr<InternalLanguage::nodes::Statement> parse_statement();

			std::unique_ptr<InternalLanguage::nodes::Block> parse_block();
			std::unique_ptr<InternalLanguage::nodes::Function> parse_function();

	};

}

#endif//MCC_PARSER_PARSER_H
