//
// Created by Max Tyson on 09/09/2026.
//

#ifndef MCC_CORE_SOURCE_LOCATION_H
#define MCC_CORE_SOURCE_LOCATION_H

#include "mcc_object.h"


#include <format>
#include <string>

namespace mcc::core {

    class SourceLocation : public  MCCObject {

        private:

            std::string m_file;
            size_t m_line;
            size_t m_col;

        public:
            SourceLocation(const std::string& file, size_t line, size_t col);
            ~SourceLocation();

            [[nodiscard]] std::string file();
            [[nodiscard]] std::string to_string() final;

    		void increment_line();
    		void increment_col();
    };

}

#endif //MCC_CORE_SOURCE_LOCATION_H
