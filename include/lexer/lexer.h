//
// Created by Max Tyson on 09/09/2026.
//

#ifndef MCC_LEXER_LEXER_H
#define MCC_LEXER_LEXER_H

#include "core/error_reporter.h"


#include <lexer/token.h>
#include <unordered_map>

namespace mcc::lexer {

	typedef struct LexerConfig {

		std::unordered_map<std::string, TokenType> keywords;

	} lexer_config_t;

	class Lexer {

		private:
			std::string m_source_code;
			size_t m_pos = 0;
			core::SourceLocation m_location;

			lexer_config_t m_config;
		    core::ErrorReporter& m_errors;

			std::deque<Token> token_buffer = {};

			char lookahead();
			char consume();
			void skip_whitespace();

			Token token_at_current(TokenType type, const std::string& lexeme);

		    Token parse_text_token(char c);
		    Token parse_digit_token(char c);
		    Token parse_comparison_token(char c);
		    Token parse_punct_token(char c);
		    Token parse_comment_token(char c);
		    Token parse_next();

		public:

			Lexer(const std::string& source, const std::string& filename, LexerConfig  config, core::ErrorReporter& reporter);
			~Lexer();

			Token next();
			Token peek(size_t offset = 0);

			[[nodiscard]] bool at_end() const;
			[[nodiscard]] std::vector<Token> tokenize_all();

	};

}

#endif//MCC_LEXER_LEXER_H
