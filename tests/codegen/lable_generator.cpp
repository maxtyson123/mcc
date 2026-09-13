//
// Created by Max Tyson on 13/09/2026.
//
#include <catch2/catch_test_macros.hpp>
#include <codegen/label_generator.h>
#include <set>
#include <cctype>

using namespace mcc::codegen;

TEST_CASE("labels are unique across many calls") {
	LabelGenerator labels;
	std::set<std::string> seen;

	for (int i = 0; i < 100; ++i)
		REQUIRE(seen.insert(labels.next()).second);
}

TEST_CASE("labels are non empty, valid identifiers") {
	LabelGenerator labels;
	std::string label = labels.next();

	REQUIRE_FALSE(label.empty());
	// NASM identifiers cannot start with a digit.
	REQUIRE_FALSE(std::isdigit(label[0]));
}