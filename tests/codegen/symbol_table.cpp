//
// Created by Max Tyson on 13/09/2026.
//

#include <catch2/catch_test_macros.hpp>
#include <codegen/symbol_table.h>

using namespace mcc::codegen;

TEST_CASE("the first symbol gets a non-zero offset") {
    // Offset 0 is reserved for not found
    SymbolTable table;
    REQUIRE(table.new_symbol("a") != 0);
}

TEST_CASE("offsets strictly increase and never collide") {
    SymbolTable table;
    size_t a = table.new_symbol("a");
    size_t b = table.new_symbol("b");
    size_t c = table.new_symbol("c");

    REQUIRE(a < b);
    REQUIRE(b < c);
}

TEST_CASE("get_offset returns the exact offset a symbol was assigned") {
    SymbolTable table;
    size_t assigned = table.new_symbol("x");
    REQUIRE(table.get_offset("x") == assigned);
}

TEST_CASE("looking up an unknown symbol returns the not-found value") {
    SymbolTable table;
    table.new_symbol("x");
    REQUIRE(table.get_offset("does_not_exist") == 0);
}

TEST_CASE("clear resets the offset counter, not just the symbol map") {
    SymbolTable table;
    table.new_symbol("a");
    table.new_symbol("b");
    table.clear();

    SymbolTable fresh;
    REQUIRE(table.new_symbol("c") == fresh.new_symbol("z"));
}

TEST_CASE("clear removes previously known symbols") {
    SymbolTable table;
    table.new_symbol("a");
    table.clear();
    REQUIRE(table.get_offset("a") == 0);
}

TEST_CASE("add_symbol installs at specified offset without advancing the counter") {
    SymbolTable table;
    table.add_symbol("manual", 999);
    REQUIRE(table.get_offset("manual") == 999);

    size_t next = table.new_symbol("auto");
    REQUIRE(next != 999);
}