//
// Created by Max Tyson on 09/09/2026.
//

#ifndef MCC_LEXER_TOKEN_H
#define MCC_LEXER_TOKEN_H

#include <string>
#include <core/source_location.h>

namespace mcc::lexer {

	enum class TokenType {
		IDENTIFIER,
		END_OF_FILE,
		ERROR,

		OPEN_PARENTHESES,
		CLOSE_PARENTHESES,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		SEMI_COLON,

		LITERAL_INTEGER,

		KEYWORD_INT,
		KEYWORD_RETURN,
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
