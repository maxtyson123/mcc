//
// Created by Max Tyson on 09/09/2026.
//

#ifndef MCC_CORE_COMPILER_ERROR_H
#define MCC_CORE_COMPILER_ERROR_H

#include <string>
#include <core/mcc_object.h>
#include <core/source_location.h>
#include <core/stage.h>

namespace mcc::core {

    enum class Severity {
        WARNING,
        ERROR,
    };

    class CompilerError : public MCCObject {

        private:
    		Stage m_stage;
            Severity m_severity;
            SourceLocation m_location;
            std::string m_message;

        public:

            CompilerError(Stage stage, Severity severity, SourceLocation location, const std::string& message);
            ~CompilerError();

            [[nodiscard]] Severity severity() const;
            [[nodiscard]] std::string to_string() final;


    };

}

#endif //MCC_CORE_COMPILER_ERROR_H
