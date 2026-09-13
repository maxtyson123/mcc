//
// Created by Max Tyson on 13/09/2026.
//

#include <catch2/catch_test_macros.hpp>
#include <lexer/lexer.h>
#include <_drivers/cpp/cpp_driver_config.h>
#include <core/error_reporter.h>
#include <cctype>

using namespace mcc::lexer;
using namespace mcc::core;
using namespace mcc::drivers::cpp;

namespace mcc::tests{
    Lexer make_lexer(const std::string& source, ErrorReporter& errors) {
        static CPPDriverConfig config;
        return Lexer(source, "test.cpp", config.lexer_config(), errors);
    }
}

TEST_CASE("single-character punctuation tokens") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("(){};+-*", errors);

    REQUIRE(lexer.next().type() == TokenType::OPEN_PARENTHESES);
    REQUIRE(lexer.next().type() == TokenType::CLOSE_PARENTHESES);
    REQUIRE(lexer.next().type() == TokenType::OPEN_BRACKET);
    REQUIRE(lexer.next().type() == TokenType::CLOSE_BRACKET);
    REQUIRE(lexer.next().type() == TokenType::SEMI_COLON);
    REQUIRE(lexer.next().type() == TokenType::ADD);
    REQUIRE(lexer.next().type() == TokenType::SUBTRACT);
    REQUIRE(lexer.next().type() == TokenType::MULTIPLY);
    REQUIRE_FALSE(errors.has_errors());
}

TEST_CASE("division vs line comment") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("/ // this is a comment\n", errors);

    REQUIRE(lexer.next().type() == TokenType::DIVIDE);
    Token comment = lexer.next();
    REQUIRE(comment.type() == TokenType::COMMENT_LINE);
    REQUIRE(comment.lexeme() == "// this is a comment");
}

TEST_CASE("assign vs equality - maximal munch") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("= ==", errors);

    REQUIRE(lexer.next().type() == TokenType::ASSIGN);
    REQUIRE(lexer.next().type() == TokenType::EQUALITY);
    REQUIRE_FALSE(errors.has_errors());
}

TEST_CASE("inequality token") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("!=", errors);
    REQUIRE(lexer.next().type() == TokenType::INEQUALITY);
    REQUIRE_FALSE(errors.has_errors());
}

TEST_CASE("a bare ! with no following = is reported as malformed") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("!", errors);
    Token t = lexer.next();
    REQUIRE(t.type() == TokenType::ERROR);
    REQUIRE(errors.has_errors());
}

TEST_CASE("keywords resolve over identifiers, and are not substrings") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("int return if else intx", errors);

    CHECK(lexer.next().type() == TokenType::KEYWORD_INT);
    CHECK(lexer.next().type() == TokenType::KEYWORD_RETURN);
    CHECK(lexer.next().type() == TokenType::CONTROL_IF);
    CHECK(lexer.next().type() == TokenType::CONTROL_ELSE);

    // "intx" must not be split into the "int" keyword plus a leftover "x"
    Token next = lexer.next();
    CHECK(next.type() == TokenType::IDENTIFIER);
    CHECK(next.lexeme() == "intx");
}

TEST_CASE("integer literal") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("42", errors);
    Token t = lexer.next();
    REQUIRE(t.type() == TokenType::LITERAL_INTEGER);
    REQUIRE(t.lexeme() == "42");
}

TEST_CASE("identifier with digits and underscore") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("_foo123 bar_2", errors);

    Token a = lexer.next();
    Token b = lexer.next();
    REQUIRE(a.type() == TokenType::IDENTIFIER);
    REQUIRE(a.lexeme() == "_foo123");
    REQUIRE(b.type() == TokenType::IDENTIFIER);
    REQUIRE(b.lexeme() == "bar_2");
}

TEST_CASE("unknown character reports an error and produces an error token, not a crash") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("@", errors);
    Token t = lexer.next();
    REQUIRE(t.type() == TokenType::ERROR);
    REQUIRE(errors.has_errors());
}

TEST_CASE("peek does not consume, and is idempotent at the same offset") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("+ -", errors);

    REQUIRE(lexer.peek(0).type() == TokenType::ADD);
    REQUIRE(lexer.peek(0).type() == TokenType::ADD); // second call must not advance
    REQUIRE(lexer.peek(1).type() == TokenType::SUBTRACT);

    REQUIRE(lexer.next().type() == TokenType::ADD);
    REQUIRE(lexer.next().type() == TokenType::SUBTRACT);
}

TEST_CASE("end of file is stable across repeated calls") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("", errors);
    REQUIRE(lexer.next().type() == TokenType::END_OF_FILE);
    REQUIRE(lexer.next().type() == TokenType::END_OF_FILE);
}

TEST_CASE("tokenize_all includes a trailing END_OF_FILE and nothing after it") {
    ErrorReporter errors;
    Lexer lexer = mcc::tests::make_lexer("+", errors);
    std::vector<Token> tokens = lexer.tokenize_all();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type() == TokenType::ADD);
    REQUIRE(tokens[1].type() == TokenType::END_OF_FILE);
}