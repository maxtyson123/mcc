//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_PARSER_PARSER_H
#define MCC_PARSER_PARSER_H


#include <il/nodes/expression.h>
#include <il/nodes/statement.h>
#include <il/nodes/control.h>
#include <il/nodes/structure.h>
#include <core/error_reporter.h>
#include <lexer/lexer.h>
#include <lexer/token.h>

namespace mcc::parser {

	class Parser {

		private:

			core::ErrorReporter& m_errors;

			std::unique_ptr<std::vector<lexer::Token>> m_tokens;
			std::vector<lexer::Token>::const_iterator m_current_token;

			lexer::Token peek();
			lexer::Token advance();

			[[nodiscard]] bool check(lexer::TokenType type);
			lexer::Token expect(lexer::TokenType type, const std::string& message);

		public:
			Parser(core::ErrorReporter& error_reporter);
			~Parser();

			void load_tokens(std::unique_ptr<std::vector<lexer::Token>> tokens);

			std::unique_ptr<InternalLanguage::nodes::LiteralInteger> parse_literal_integer();
			std::unique_ptr<InternalLanguage::nodes::VariableReference> parse_variable_reference();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_value_expression();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_composite_expression_higher();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_composite_expression_mid();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_composite_expression_lower();
			std::unique_ptr<InternalLanguage::nodes::Expression> parse_expression();

			std::unique_ptr<InternalLanguage::nodes::StatementReturn> parse_return();
		    std::unique_ptr<InternalLanguage::nodes::Statement> parse_control_statement();
		    std::unique_ptr<InternalLanguage::nodes::Statement> parse_statement();

			std::unique_ptr<InternalLanguage::nodes::Block> parse_block();

			std::unique_ptr<InternalLanguage::nodes::FunctionDeclaration> parse_function_declaration(lexer::Token type_token, lexer::Token name_token);
			std::unique_ptr<InternalLanguage::nodes::VariableDeclaration> parse_variable_declaration(lexer::Token type_token, lexer::Token name_token);
			std::unique_ptr<InternalLanguage::nodes::Declaration> parse_declaration();

			std::unique_ptr<InternalLanguage::nodes::Program> parse_program();

	};

}

#endif//MCC_PARSER_PARSER_H
