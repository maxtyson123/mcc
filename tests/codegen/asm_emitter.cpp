//
// Created by Max Tyson on 13/09/2026.
//

#include <catch2/catch_test_macros.hpp>
#include <codegen/asm_emitter.h>
#include <parser/parser.h>
#include <lexer/lexer.h>
#include <_drivers/cpp/cpp_driver_config.h>
#include <core/error_reporter.h>

using namespace mcc::lexer;
using namespace mcc::parser;
using namespace mcc::core;
using namespace mcc::codegen;
using namespace mcc::drivers::cpp;
using namespace mcc::InternalLanguage::nodes;

namespace {
    std::string compile(const std::string& source, ErrorReporter& errors) {
        CPPDriverConfig config;
        Lexer lexer(source, "test.cpp", config.lexer_config(), errors);
        std::vector<Token> tokens = lexer.tokenize_all();

        Parser parser(errors);
        parser.load_tokens(std::make_unique<std::vector<Token>>(tokens));
        std::unique_ptr<Program> program = parser.parse_program();

        AsmEmitter emitter;
        return emitter.emit_program(*program);
    }

    bool contains(const std::string& haystack, const std::string& needle) {
        return haystack.find(needle) != std::string::npos;
    }
}

TEST_CASE("main is exported and labelled") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { return 42; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "global main"));
    REQUIRE(contains(asm_out, "main:"));
}

TEST_CASE("every function has exactly one ret") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { if (1 == 1) { return 1; } return 0; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    size_t count = 0;
    size_t pos = 0;
    while ((pos = asm_out.find("ret", pos)) != std::string::npos) {
        ++count;
        pos += 3;
    }

	// 2 as includes _setup's ret
    REQUIRE(count == 2);
}

TEST_CASE("a function with no locals does not reserve any stack space") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { return 42; }", errors);
    REQUIRE_FALSE(errors.has_errors());
    REQUIRE(contains(asm_out, "sub rsp, 0"));
}

TEST_CASE("a function with a local reserves stack space for it") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { int x = 1; return x; }", errors);
    REQUIRE_FALSE(errors.has_errors());
    REQUIRE(contains(asm_out, "sub rsp, 8"));
}

TEST_CASE("addition emits an add instruction") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { return 1 + 2; }", errors);
    REQUIRE_FALSE(errors.has_errors());
    REQUIRE(contains(asm_out, "add rax, rbx"));
}

TEST_CASE("subtraction emits a sub instruction") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { return 5 - 2; }", errors);
    REQUIRE_FALSE(errors.has_errors());
    REQUIRE(contains(asm_out, "sub rax, rbx"));
}

TEST_CASE("comparison operators emits set-condition instructions") {
    struct Case { std::string source; std::string expected_mnemonic; };
    const Case cases[] = {
        {"1 == 2", "sete"},
        {"1 != 2", "setne"},
        {"1 < 2",  "setl"},
        {"1 <= 2", "setle"},
        {"1 > 2",  "setg"},
        {"1 >= 2", "setge"},
    };

    for (const auto& c : cases) {
        ErrorReporter errors;
        std::string asm_out = compile("int main() { return " + c.source + "; }", errors);
        REQUIRE_FALSE(errors.has_errors());
        CHECK(contains(asm_out, c.expected_mnemonic));
    }
}

TEST_CASE("a global variable is placed in .bss and addressed by label") {
    ErrorReporter errors;
    std::string asm_out = compile("int g = 5; int main() { return g; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "section .bss"));
    REQUIRE(contains(asm_out, "g:"));
    REQUIRE(contains(asm_out, "[rel g]"));
}

TEST_CASE("a local variable is addressed relative to rbp isntead of lable") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { int x = 5; return x; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "[rbp - 8]"));
    REQUIRE_FALSE(contains(asm_out, "[rel x]"));
}

TEST_CASE("a while loop jumps back to its own start label") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { while (1 < 2) { return 0; } return 1; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "jmp"));
    REQUIRE(contains(asm_out, "je"));
}

TEST_CASE("an if-else emits a jump around the else branch") {
    ErrorReporter errors;
    std::string asm_out = compile(
        "int main() { if (1 == 2) { return 1; } else { return 2; } }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "je"));
    REQUIRE(contains(asm_out, "jmp"));
}

TEST_CASE("a global initialiser runs before main via setup function") {
    ErrorReporter errors;
    std::string asm_out = compile("int g = 5; int main() { return g; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "_setup:"));
    REQUIRE(contains(asm_out, "global _setup"));
}


TEST_CASE("assigning to a local variable writes to its rbp-relative slot") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { int x = 1; x = 2; return x; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "mov rax, 2"));
    REQUIRE(contains(asm_out, "mov [rbp - 8], rax"));
}

TEST_CASE("assigning to a global variable writes to its label, not a stack slot") {
    ErrorReporter errors;
    std::string asm_out = compile("int g = 1; int main() { g = 2; return g; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(contains(asm_out, "[rel g], rax"));
}

TEST_CASE("an assignment's right-hand side is fully evaluated before the store") {
    ErrorReporter errors;
    std::string asm_out = compile("int main() { int x = 1; x = 1 + 2; return x; }", errors);
    REQUIRE_FALSE(errors.has_errors());

	size_t add_pos = asm_out.find("add rax, rbx");
    size_t store_pos = asm_out.find("mov [rbp - 8], rax", add_pos);

    REQUIRE(add_pos != std::string::npos);
    REQUIRE(store_pos != std::string::npos);
    REQUIRE(store_pos > add_pos);
}