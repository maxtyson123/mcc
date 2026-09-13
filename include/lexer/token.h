//
// Created by Max Tyson on 09/09/2026.
//

#ifndef MCC_LEXER_TOKEN_H
#define MCC_LEXER_TOKEN_H

#include <string>
#include <core/source_location.h>

namespace mcc::lexer {
	enum class TokenType {
		END_OF_FILE,
		ERROR,

		IDENTIFIER,
		ASSIGN,

		OPEN_PARENTHESES,
		CLOSE_PARENTHESES,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		SEMI_COLON,

		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,

		EQUALITY,
		INEQUALITY,
		LESS_THAN,
		LESS_THAN_EQ,
		MORE_THAN,
		MORE_THAN_EQ,

		CONTROL_IF,
		CONTROL_ELSE,
		CONTROL_WHILE,

		LITERAL_INTEGER,

		KEYWORD_INT,
		KEYWORD_RETURN,

		COMMENT_LINE,
	};


	class Token : core::MCCObject {

		private:
			TokenType m_type;
			std::string m_lexeme;
			core::SourceLocation m_location;

		public:
			Token(TokenType type, std::string lexeme, core::SourceLocation location);
			~Token();

			[[nodiscard]] TokenType type() const;
			[[nodiscard]] std::string lexeme();
			[[nodiscard]] core::SourceLocation location();

            [[nodiscard]] std::string to_string() final;

	};

}

#endif//MCC_LEXER_TOKEN_H
