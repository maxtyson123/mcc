//
// Created by Max Tyson on 10/09/2026.
//

#include <codegen/label_generator.h>

using namespace mcc;
using namespace mcc::codegen;

LabelGenerator::LabelGenerator() = default;

LabelGenerator::~LabelGenerator() = default;

std::string LabelGenerator::next() {

	return std::format("_mcc_L{}", m_next_label++);
}