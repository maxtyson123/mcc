//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_DRIVER_DRIVER_CONFIG_H
#define MCC_DRIVER_DRIVER_CONFIG_H

#include <lexer/lexer.h>

namespace mcc::driver {

	class DriverConfig {

		private:
			lexer::LexerConfig m_lexer_config;

		public:
			DriverConfig(const lexer::LexerConfig& lexer_config);
			~DriverConfig();

			[[nodiscard]] lexer::LexerConfig lexer_config();
	};

}

#endif//MCC_DRIVER_DRIVER_CONFIG_H
