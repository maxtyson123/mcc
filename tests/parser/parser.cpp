//
// Created by Max Tyson on 13/09/2026.
//

#include <catch2/catch_test_macros.hpp>
#include <parser/parser.h>
#include <lexer/lexer.h>
#include <_drivers/cpp/cpp_driver_config.h>
#include <core/error_reporter.h>

using namespace mcc::lexer;
using namespace mcc::parser;
using namespace mcc::core;
using namespace mcc::drivers::cpp;
using namespace mcc::InternalLanguage::nodes;

namespace mcc::tests {

    std::unique_ptr<Program> parse_source(const std::string& source, ErrorReporter& errors) {
        CPPDriverConfig config;
        Lexer lexer(source, "test.cpp", config.lexer_config(), errors);
        std::vector<Token> tokens = lexer.tokenize_all();

        Parser parser(errors);
        parser.load_tokens(std::make_unique<std::vector<Token>>(tokens));
        return parser.parse_program();
    }

    // Grabs the first function declaration's body statements from a program.
    std::vector<std::unique_ptr<Statement>>& main_body(Program& program) {
        auto& decl = program.declarations()[0];
        auto& fn = (FunctionDeclaration&)(*decl);
        return fn.body()->statements();
    }
}

TEST_CASE("multiply binds tighter than add") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return 2 + 3 * 4; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    auto& add = (BinaryOperation&)(*ret.return_value());

    REQUIRE(add.op() == BinaryOperator::ADD);
    REQUIRE(add.left()->type() == NodeType::LITERAL_INTEGER);
    REQUIRE(add.right()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(((BinaryOperation&)(*add.right())).op() == BinaryOperator::MULTIPLY);
}

TEST_CASE("division binds as tightly as multiplication") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return 8 / 2 * 3; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    // Same precedence tier, so this must still be left-associative: (8 / 2) * 3
    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    auto& outer = (BinaryOperation&)(*ret.return_value());

    REQUIRE(outer.op() == BinaryOperator::MULTIPLY);
    REQUIRE(outer.left()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(((BinaryOperation&)(*outer.left())).op() == BinaryOperator::DIVIDE);
}

TEST_CASE("subtraction is left-associative") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return 10 - 2 - 3; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    // Must be (10 - 2) - 3, not 10 - (2 - 3)
    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    auto& outer = (BinaryOperation&)(*ret.return_value());

    REQUIRE(outer.op() == BinaryOperator::SUBTRACT);
    REQUIRE(outer.left()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(outer.right()->type() == NodeType::LITERAL_INTEGER);

    auto& inner = (BinaryOperation&)(*outer.left());
    REQUIRE(inner.op() == BinaryOperator::SUBTRACT);
    REQUIRE(((LiteralInteger&)(*inner.left())).value() == 10);
    REQUIRE(((LiteralInteger&)(*inner.right())).value() == 2);
    REQUIRE(((LiteralInteger&)(*outer.right())).value() == 3);
}

TEST_CASE("parentheses override precedence") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return (1 + 2) * 3; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    auto& mul = (BinaryOperation&)(*ret.return_value());

    REQUIRE(mul.op() == BinaryOperator::MULTIPLY);
    REQUIRE(mul.left()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(((BinaryOperation&)(*mul.left())).op() == BinaryOperator::ADD);
}

TEST_CASE("nested parentheses parse to the same tree as no parentheses at all") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return ((1 + 2)); }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    REQUIRE(ret.return_value()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(((BinaryOperation&)(*ret.return_value())).op() == BinaryOperator::ADD);
}

TEST_CASE("comparisons bind looser than arithmetic") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return 1 + 2 == 3; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    auto& eq = (BinaryOperation&)(*ret.return_value());

    REQUIRE(eq.op() == BinaryOperator::EQUALITY);
    REQUIRE(eq.left()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(((BinaryOperation&)(*eq.left())).op() == BinaryOperator::ADD);
    REQUIRE(eq.right()->type() == NodeType::LITERAL_INTEGER);
}

TEST_CASE("all comparison operators parse to their own operator") {
    struct Case { std::string source; BinaryOperator expected; };
    const Case cases[] = {
        {"1 == 2", BinaryOperator::EQUALITY},
        {"1 != 2", BinaryOperator::INEQUALITY},
        {"1 < 2",  BinaryOperator::LESS_THAN},
        {"1 <= 2", BinaryOperator::LESS_THAN_EQ},
        {"1 > 2",  BinaryOperator::MORE_THAN},
        {"1 >= 2", BinaryOperator::MORE_THAN_EQ},
    };

    for (const auto& c : cases) {
        ErrorReporter errors;
        auto program = mcc::tests::parse_source("int main() { return " + c.source + "; }", errors);
        REQUIRE_FALSE(errors.has_errors());

        auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
        REQUIRE(ret.return_value()->type() == NodeType::BINARY_OPERATION);
        CHECK(((BinaryOperation&)(*ret.return_value())).op() == c.expected);
    }
}

TEST_CASE("variable reference") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return x; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& ret = (StatementReturn&)(*mcc::tests::main_body(*program)[0]);
    REQUIRE(ret.return_value()->type() == NodeType::VARIABLE_REFERENCE);
    REQUIRE(((VariableReference&)(*ret.return_value())).name() == "x");
}

TEST_CASE("variable declaration with and without an initialiser") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { int x; int y = 5; return y; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& body = mcc::tests::main_body(*program);
    auto& x = (VariableDeclaration&)(*body[0]);
    auto& y = (VariableDeclaration&)(*body[1]);

    REQUIRE(x.name() == "x");
    REQUIRE(x.initialiser() == nullptr);

    REQUIRE(y.name() == "y");
    REQUIRE(y.initialiser() != nullptr);
    REQUIRE(((LiteralInteger&)(*y.initialiser())).value() == 5);
}

TEST_CASE("a global variable and a function can exist at program scope") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int g = 1; int main() { return g; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(program->declarations().size() == 2);
    REQUIRE(program->declarations()[0]->type() == NodeType::DECLARATION_VARIABLE);
    REQUIRE(program->declarations()[1]->type() == NodeType::DECLARATION_FUNCTION);
    REQUIRE(((VariableDeclaration&)(*program->declarations()[0])).name() == "g");
}

TEST_CASE("a bare compound block is a valid statement") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { { int z = 1; } return 0; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& body = mcc::tests::main_body(*program);
    REQUIRE(body[0]->type() == NodeType::BLOCK);

    auto& inner = (Block&)(*body[0]);
    REQUIRE(inner.statements().size() == 1);
    REQUIRE(inner.statements()[0]->type() == NodeType::DECLARATION_VARIABLE);
}

TEST_CASE("if without an else branch") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { if (1 == 1) { return 1; } return 0; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& body = mcc::tests::main_body(*program);
    REQUIRE(body[0]->type() == NodeType::IF_ELSE);

    auto& if_stmt = (IfElseStatement&)(*body[0]);
    REQUIRE_FALSE(if_stmt.has_else());
    REQUIRE(if_stmt.condition()->type() == NodeType::BINARY_OPERATION);
    REQUIRE(if_stmt.body()->type() == NodeType::BLOCK);
}

TEST_CASE("if with an else branch") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { if (1 == 2) { return 1; } else { return 2; } }", errors);
    REQUIRE_FALSE(errors.has_errors());

    auto& if_stmt = (IfElseStatement&)(*mcc::tests::main_body(*program)[0]);
    REQUIRE(if_stmt.has_else());
    REQUIRE(if_stmt.else_body()->type() == NodeType::BLOCK);
}

TEST_CASE("while loop parses its condition and body") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { while (1 < 2) { return 0; } return 1; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(mcc::tests::main_body(*program)[0]->type() == NodeType::LOOP_WHILE);
}

TEST_CASE("a line comment is its own statement") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { // a note\n return 0; }", errors);
    REQUIRE_FALSE(errors.has_errors());

    REQUIRE(mcc::tests::main_body(*program)[0]->type() == NodeType::COMMENT);
}

TEST_CASE("a malformed statement reports an error but does not crash the parser") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return ; }", errors);

    REQUIRE(errors.has_errors());
    // The parser must still hand back a Program, not crash or return nullptr
    // for the whole compile just because one statement was malformed.
    REQUIRE(program != nullptr);
}

TEST_CASE("a missing closing parenthesis is reported, not silently accepted") {
    ErrorReporter errors;
    auto program = mcc::tests::parse_source("int main() { return (1 + 2; }", errors);
    REQUIRE(errors.has_errors());
}