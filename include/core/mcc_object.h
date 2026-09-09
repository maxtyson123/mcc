//
// Created by Max Tyson on 10/09/2026.
//

#ifndef MCC_CORE_MCCOBJECT_H
#define MCC_CORE_MCCOBJECT_H

#include <string>

namespace mcc::core {

	class MCCObject {

		public:
			[[nodiscard]] virtual std::string to_string() = 0;


	};

}

#endif//MCC_CORE_MCCOBJECT_H
