//
// Created by Max Tyson on 10/09/2026.
//

#include <_drivers/cpp/cpp_driver_config.h>

using namespace mcc;
using namespace mcc::lexer;
using namespace mcc::driver;
using namespace mcc::drivers;
using namespace mcc::drivers::cpp;

CPPDriverConfig::CPPDriverConfig()
: DriverConfig(build_lexer_config())
{

}

CPPDriverConfig::~CPPDriverConfig() = default;

LexerConfig CPPDriverConfig::build_lexer_config() {
	return {
		{
			{"int", TokenType::KEYWORD_INT},
			{"return", TokenType::KEYWORD_RETURN},
			{"if", TokenType::CONTROL_IF},
			{"else", TokenType::CONTROL_ELSE},
			{"when", TokenType::CONTROL_WHILE},
		}
	};
}