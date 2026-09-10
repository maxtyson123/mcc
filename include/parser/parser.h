//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_PARSER_PARSER_H
#define MCC_PARSER_PARSER_H


#include <core/error_reporter.h>
#include <lexer/lexer.h>
#include <lexer/token.h>
#include <il/nodes.h>

namespace mcc::parser {

	class Parser {

		private:

			lexer::Lexer m_lexer;
			core::ErrorReporter m_errors;

			lexer::Token m_current_token;

			lexer::Token peek();
			lexer::Token advance();

			[[nodiscard]] bool check(lexer::TokenKind kind);
			lexer::Token expect(lexer::TokenKind kind, const std::string& message);

		public:
			Parser(const lexer::Lexer& lexer, const core::ErrorReporter& error_reporter);
			~Parser();


			std::unique_ptr<InternalLanguage::LiteralInteger> parse_literal_integer();
			std::unique_ptr<InternalLanguage::Expression> parse_expression();
			std::unique_ptr<InternalLanguage::StatementReturn> parse_return();
			std::unique_ptr<InternalLanguage::Statement> parse_statement();
			std::unique_ptr<InternalLanguage::Block> parse_block();
			std::unique_ptr<InternalLanguage::Function> parse_function();

	};

}

#endif//MCC_PARSER_PARSER_H
