//
// Created by Max Tyson on 13/09/2026.
//

#include <catch2/catch_test_macros.hpp>
#include <codegen/frame_allocator.h>
#include <codegen/symbol_table.h>
#include <il/nodes/structure.h>

using namespace mcc::codegen;
using namespace mcc::InternalLanguage::nodes;

namespace {
    std::unique_ptr<VariableDeclaration> local(const std::string& name) {
        return std::make_unique<VariableDeclaration>(name, nullptr);
    }
}

TEST_CASE("a function with no locals needs no stack space") {
    std::vector<std::unique_ptr<Statement>> stmts;
    Block block(std::move(stmts));

    SymbolTable table;
    FrameAllocator allocator;
    REQUIRE(allocator.allocate(block, table) == 0);
}

TEST_CASE("total space matches the number of locals") {
    std::vector<std::unique_ptr<Statement>> stmts;
    stmts.push_back(local("a"));
    stmts.push_back(local("b"));
    stmts.push_back(local("c"));
    Block block(std::move(stmts));

    SymbolTable table;
    FrameAllocator allocator;

    REQUIRE(allocator.allocate(block, table) == 24);
}

TEST_CASE("every declared local gets its own distinct offset") {
    std::vector<std::unique_ptr<Statement>> stmts;
    stmts.push_back(local("a"));
    stmts.push_back(local("b"));
    Block block(std::move(stmts));

    SymbolTable table;
    FrameAllocator allocator;
    allocator.allocate(block, table);

    REQUIRE(table.get_offset("a") != 0);
    REQUIRE(table.get_offset("b") != 0);
    REQUIRE(table.get_offset("a") != table.get_offset("b"));
}

TEST_CASE("a local declared inside a nested block still gets stack space") {
    std::vector<std::unique_ptr<Statement>> inner_stmts;
    inner_stmts.push_back(local("inner"));
    auto inner_block = std::make_unique<Block>(std::move(inner_stmts));

    std::vector<std::unique_ptr<Statement>> outer_stmts;
    outer_stmts.push_back(std::move(inner_block));
    Block outer_block(std::move(outer_stmts));

    SymbolTable table;
    FrameAllocator allocator;
    allocator.allocate(outer_block, table);

    REQUIRE(table.get_offset("inner") != 0);
}

TEST_CASE("a statement that is not a declaration does not consume any stack space") {
    std::vector<std::unique_ptr<Statement>> stmts;
    stmts.push_back(std::make_unique<StatementReturn>(std::make_unique<LiteralInteger>(0)));
    Block block(std::move(stmts));

    SymbolTable table;
    FrameAllocator allocator;
    REQUIRE(allocator.allocate(block, table) == 0);
}