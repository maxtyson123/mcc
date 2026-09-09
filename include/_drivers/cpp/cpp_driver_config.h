//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_DRIVERS_CPP_CPP_DRIVER_CONFIG_H
#define MCC_DRIVERS_CPP_CPP_DRIVER_CONFIG_H

#include <driver/driver_config.h>

namespace mcc::drivers::cpp {

	class CPPDriverConfig : public driver::DriverConfig{

		private:
			static lexer::LexerConfig build_lexer_config();

		public:
			CPPDriverConfig();
			~CPPDriverConfig();

	};

}

#endif//MCC_DRIVERS_CPP_CPP_DRIVER_CONFIG_H
