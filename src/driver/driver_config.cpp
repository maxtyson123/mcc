//
// Created by Max Tyson on 10/09/2026.
//

#include <driver/driver_config.h>

using namespace mcc;
using namespace mcc::driver;
using namespace mcc::lexer;

DriverConfig::DriverConfig(const LexerConfig& lexer_config)
: m_lexer_config(lexer_config)
{

}

DriverConfig::~DriverConfig() = default;

LexerConfig DriverConfig::lexer_config() {
	return m_lexer_config;
}