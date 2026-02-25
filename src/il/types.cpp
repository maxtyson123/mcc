/**
 * @file types.cpp
 * @brief Implements the type used by mcc's internal language
 *
 * @author Max Tyson
 * @date 24th February 2026
 */

#include <il/types.h>

#include <utility>

using namespace mcc;
using namespace mcc::InternalLanguage;

/**
 * @brief Creates the base Type object by storing the given classification
 *
 * @param classification The type of this type
 */
Type::Type(Classification classification)
: m_class(classification)
{

}

Type::~Type() = default;

/**
 * @brief Gets the classification of this type
 *
 * @return The classification of this type
 */
Classification Type::classification() const {
    return m_class;
}

/**
 * @brief Get the string representation of this type
 *
 * @return This type's information in string form
 */
std::string Type::to_string() const {
    return "Unknown";
}

/**
 * @brief Creates the Integer by storing the signedness and bit width
 *
 * @param is_signed Whether this integer type can be used to store negative values
 * @param bit_size How many bits are used to store the value of this number
 */
IntegerType::IntegerType(bool is_signed, BitSize bit_size)
: Type(Classification::INTEGER),
  m_is_signed(is_signed),
  m_bit_size(bit_size)
{
}

IntegerType::~IntegerType() = default;

/**
 * @brief Get the string representation of this type
 *
 * @return This type's information in string form
 */
std::string IntegerType::to_string() const {

    std::string base = "int";

    // Add the bitsize
    switch (m_bit_size) {
        case BitSize::BITS_8:
            base += "8b";
            break;

        case BitSize::BITS_16:
            base += "16b";
            break;

        case BitSize::BITS_32:
            base += "32b";
            break;

        case BitSize::BITS_64:
            base += "64b";
            break;
    }

    return base;
}

/**
 * @brief Get whether the integer can store negative values
 *
 * @return True if it can, false otherwise
 */
bool IntegerType::is_signed() const {
    return m_is_signed;
}

/**
 * @brief Get the bit width of the number
 *
 * @return The bitwith
 */
BitSize IntegerType::bit_size() const {
    return m_bit_size;
}

/**
 * @brief Creates the Decimal by storing the signedness and bit width
 *
 * @param is_signed Whether this decimal type can be used to store negative values
 * @param bit_size How many bits are used to store the value of this number
 */
DecimalType::DecimalType(bool is_signed, BitSize bit_size)
: Type(Classification::DECIMAL),
  m_is_signed(is_signed),
  m_bit_size(bit_size)
{

}

DecimalType::~DecimalType() = default;

/**
 * @brief Get the string representation of this type
 *
 * @return This type's information in string form
 */
std::string DecimalType::to_string() const {

    std::string base = "dec";

    switch (m_bit_size) {
        case BitSize::BITS_8:
            base += "8b";
            break;
        case BitSize::BITS_16:
            base += "16b";
            break;
        case BitSize::BITS_32:
            base += "32b";
            break;
        case BitSize::BITS_64:
            base += "64b";
            break;
    }

    return base;

}

/**
 * @brief Get whether the decimal can store negative values
 *
 * @return True if it can, false otherwise
 */
bool DecimalType::is_signed() const {
    return m_is_signed;
}

/**
 * @brief Get the bit width of the number
 *
 * @return The bitwith
 */
BitSize DecimalType::bit_size() const {
    return m_bit_size;
}

/**
 * @brief Creates a Boolean.
 */
BooleanType::BooleanType()
: Type(Classification::BOOLEAN)
{
}

BooleanType::~BooleanType() = default;

std::string BooleanType::to_string() const {
    return "bool";
}

PointerType::PointerType(Type* pointer)
: Type(Classification::POINTER),
  m_pointer_type(pointer)
{

}

PointerType::~PointerType() = default;

std::string PointerType::to_string() const {
    return m_pointer_type->to_string() + "*";
}

Type* PointerType::pointer() const {
    return m_pointer_type;
}

ArrayType::ArrayType(Type* array_type, size_t size, bool fixed_size)
: Type(Classification::ARRAY),
  m_array_type(array_type),
  m_size(size),
  m_fixed_size(fixed_size)
{
}

ArrayType::~ArrayType() = default;

std::string ArrayType::to_string() const {

    // Add the array brackets
    std::string base = "[]";
    if (m_fixed_size)
        base = "[" + std::to_string(m_size) + "]";

    return array_type()->to_string() + base;
}

Type* ArrayType::array_type() const {
    return m_array_type;
}

size_t ArrayType::size() const {
    return m_size;
}

bool ArrayType::fixed_size() const {
    return m_fixed_size;
}

FunctionType::FunctionType(Type* return_type, std::vector<Type*> arg_types)
: Type(Classification::FUNCTION),
  m_return_type(return_type),
  m_arg_types(std::move(arg_types))
{
}

FunctionType::~FunctionType() = default;

std::string FunctionType::to_string() const {

    // Build the arg string
    std::string args;
    for (auto arg_type : m_arg_types)
        args += arg_type->to_string() + ", ";

    // Remove the last ", "
    if (!args.empty())
        args.resize(args.size() - 2);

    return "function(" + args + ")->" + return_type()->to_string();
}

Type* FunctionType::return_type() const {
    return m_return_type;
}

std::vector<Type*> FunctionType::arg_types() const {
    return m_arg_types;
}

NullType::NullType()
: Type(Classification::VOID)
{

}

NullType::~NullType() = default;

std::string NullType::to_string() const {
    return "null";
}

GenericParameterType::GenericParameterType(std::string name, std::vector<Type*> constraints)
: Type(Classification::GENERIC_PARAMETER),
  m_name(std::move(name)),
  m_constraints(std::move(constraints))
{

}

GenericParameterType::~GenericParameterType() = default;

std::string GenericParameterType::to_string() const {
    return "todo generic stringrpr";
}

std::string GenericParameterType::parameter_name() const {
    return m_name;
}

std::vector<Type*> GenericParameterType::constraints() const {
    return m_constraints;
}

StructType::StructType(std::vector<struct_type_entry_t> entries)
: Type(Classification::STRUCT),
  m_entries(std::move(entries))
{
}

StructType::~StructType() = default;

std::string StructType::to_string() const {

    std::string fields;
    for (auto field : m_entries)
        fields += field.m_type->to_string() + " " + field.name + ", ";

    if (!fields.empty())
        fields.resize(fields.size() - 2);

    return "{" + fields + "}";
}

std::vector<struct_type_entry_t> StructType::entries() const {
    return m_entries;
}



