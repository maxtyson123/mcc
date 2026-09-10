//
// Created by Max Tyson on 09/09/2026.
//

#ifndef MCC_CORE_ERROR_REPORTER_H
#define MCC_CORE_ERROR_REPORTER_H

#include <string>
#include <core/compiler_error.h>

namespace mcc::core {

    class ErrorReporter {

        private:
            std::vector<CompilerError> m_errors;

        public:
            ErrorReporter();
            ~ErrorReporter();

            void report(Stage stage, Severity severity, const SourceLocation& location, const std::string& message);
    		void fatal(const std::string& message);

            [[nodiscard]] bool has_errors();
            [[nodiscard]] const std::vector<CompilerError>& errors();

            void print_all();

    };
}

#endif //MCC_CORE_ERROR_REPORTER_H
