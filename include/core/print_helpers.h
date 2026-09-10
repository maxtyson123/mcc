//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_PRINT_HELPERS_H
#define MCC_PRINT_HELPERS_H

#include <lexer/token.h>
#include <core/error_reporter.h>
#include <il/nodes/expression.h>

namespace std {

	template <> struct formatter<mcc::lexer::TokenType> : formatter<std::string_view> {
		auto format(mcc::lexer::TokenType type, std::format_context& ctx) const {
			std::string_view name = "Unknown";
			switch (type) {
				case mcc::lexer::TokenType::IDENTIFIER:  name = "Identifer"; break;
				case mcc::lexer::TokenType::END_OF_FILE: name = "End Of File"; break;
				case mcc::lexer::TokenType::ERROR: name = "Error"; break;

				case mcc::lexer::TokenType::OPEN_PARENTHESES:  name = "Open Parentheses"; break;
				case mcc::lexer::TokenType::CLOSE_PARENTHESES: name = "Close Parentheses"; break;
				case mcc::lexer::TokenType::OPEN_BRACKET: name = "Open Bracket"; break;
				case mcc::lexer::TokenType::CLOSE_BRACKET: name = "Close Bracket"; break;
				case mcc::lexer::TokenType::SEMI_COLON: name = "Semi Colon"; break;

				case mcc::lexer::TokenType::ADD: name = "Add"; break;
				case mcc::lexer::TokenType::SUBTRACT: name = "Subtract"; break;
				case mcc::lexer::TokenType::MULTIPLY: name = "Multiply"; break;
				case mcc::lexer::TokenType::DIVIDE: name = "Divide"; break;

				case mcc::lexer::TokenType::LITERAL_INTEGER:  name = "Integer Literal"; break;

				case mcc::lexer::TokenType::KEYWORD_INT: name = "Int Keyword"; break;
				case mcc::lexer::TokenType::KEYWORD_RETURN: name = "Return Keyword"; break;
			}
			return std::formatter<std::string_view>::format(name, ctx);
		}
	};


	template <> struct formatter<mcc::core::Severity> : formatter<std::string_view> {
		auto format(mcc::core::Severity severity, std::format_context& ctx) const {
			std::string_view name = "Unknown";
			switch (severity) {
				case mcc::core::Severity::ERROR:  name = "Error"; break;
				case mcc::core::Severity::WARNING:  name = "Warning"; break;
			}
			return std::formatter<std::string_view>::format(name, ctx);
		}
	};

	template <> struct formatter<mcc::core::Stage> : formatter<std::string_view> {
		auto format(mcc::core::Stage stage, std::format_context& ctx) const {
			std::string_view name = "Unknown";
			switch (stage) {
				case mcc::core::Stage::SETUP:  name = "Setup"; break;
				case mcc::core::Stage::LEXER:  name = "Lexer"; break;
				case mcc::core::Stage::PARSER:  name = "Parser"; break;
			}
			return std::formatter<std::string_view>::format(name, ctx);
		}
	};

	template <> struct formatter<mcc::InternalLanguage::nodes::BinaryOperator> : formatter<std::string_view> {
		auto format(mcc::InternalLanguage::nodes::BinaryOperator op, std::format_context& ctx) const {
			std::string_view name = "Unknown";
			switch (op) {
				case mcc::InternalLanguage::nodes::BinaryOperator::ADD:  name = "Add"; break;
				case mcc::InternalLanguage::nodes::BinaryOperator::SUBTRACT:  name = "Subtract"; break;
				case mcc::InternalLanguage::nodes::BinaryOperator::MULTIPLY:  name = "Multiply"; break;
				case mcc::InternalLanguage::nodes::BinaryOperator::DIVIDE:  name = "Divide"; break;
			}
			return std::formatter<std::string_view>::format(name, ctx);
		}
	};

}

#endif//MCC_PRINT_HELPERS_H
