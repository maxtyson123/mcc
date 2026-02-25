/**
 * @file types.cpp
 * @brief Implements the type used by mcc's internal language
 *
 * @author Max Tyson
 * @date 24th February 2026
 */

#ifndef MCC_INTERNAL_LANGUAGE_TYPES_H
#define MCC_INTERNAL_LANGUAGE_TYPES_H
#include <cstdint>
#include <vector>

namespace mcc::InternalLanguage {

    /**
     * @enum BitSize
     * @brief Defines the bit width of a number type
     */
    enum class BitSize : uint8_t {
        BITS_8,
        BITS_16,
        BITS_32,
        BITS_64,
    };

    /**
     * @enum Classification
     * @brief Defines the specific type for a generic type object
     */
    enum class Classification {
        INTEGER,
        DECIMAL,
        BOOLEAN,
        POINTER,
        ARRAY,
        FUNCTION,
        VOID,
        GENERIC_PARAMETER,
        STRUCT,
    };

    /**
     * @class Type
     * @brief Defines a generic class to hold semantic metadata
     */
    class Type {

        private:
            Classification m_class;

        public:
            Type(Classification classification);
            virtual ~Type();

            [[nodiscard]] Classification classification() const;

            [[nodiscard]] virtual std::string to_string() const;
    };

    /**
    * @class IntegerType
    * @brief Defines the semantic metadata for a whole number
    */
    class IntegerType : public Type {

        private:
            bool m_is_signed;
            BitSize m_bit_size;

        public:
            IntegerType(bool is_signed, BitSize bit_size);
            ~IntegerType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] bool is_signed() const;
            [[nodiscard]] BitSize bit_size() const;
    };

    /**
    * @class DecimalType
    * @brief Defines the semantic metadata for a fraction
    */
    class DecimalType : public Type {

        private:
            bool m_is_signed;
            BitSize m_bit_size;

        public:
            DecimalType(bool is_signed, BitSize bit_size);
            ~DecimalType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] bool is_signed() const;
            [[nodiscard]] BitSize bit_size() const;

    };

    /**
    * @class BooleanType
    * @brief Defines the semantic metadata for a true or false value
    */
    class BooleanType : public Type {

        public:
            BooleanType();
            ~BooleanType() override;

            [[nodiscard]] std::string to_string() const final;

    };

    /**
    * @class PointerType
    * @brief Defines the semantic metadata for a reference to another type
    */
    class PointerType : public Type {

        private:
            Type* m_pointer_type;

        public:
            PointerType(Type* pointer);
            ~PointerType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] Type* pointer() const;

    };

    /**
    * @class ArrayType
    * @brief Defines the semantic metadata for a list of other types.
    *
    * @warning size() is only available if a fixed array, test with fixed_size()
    */
    class ArrayType : public Type {

        private:
            Type* m_array_type;
            size_t m_size;
            bool m_fixed_size;

        public:
            ArrayType(Type* array_type, size_t size, bool fixed_size);
            ~ArrayType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] Type* array_type() const;
            [[nodiscard]] size_t size() const;
            [[nodiscard]] bool fixed_size() const;

    };

    /**
    * @class FunctionType
    * @brief Defines the semantic metadata for a callable method
    */
    class FunctionType : public Type {

        private:
            Type* m_return_type;
            std::vector<Type*> m_arg_types;

        public:
            FunctionType(Type* return_type, std::vector<Type*> arg_types);
            ~FunctionType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] Type* return_type() const;
            [[nodiscard]] std::vector<Type*> arg_types() const;

    };

    /**
    * @class NullType
    * @brief Defines the semantic metadata for no type
    */
    class NullType : public Type {

        public:
            NullType();
            ~NullType() override;

            [[nodiscard]] std::string to_string() const final;

    };

    /**
     * @class GenericParameterType
     * @brief Defines the semantic metadata for
     */
    class GenericParameterType : public Type {
        private:
            std::string m_name;
            std::vector<Type*> m_constraints;

        public:
            GenericParameterType(std::string name, std::vector<Type*> constraints);
            ~GenericParameterType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] std::string parameter_name() const;
            [[nodiscard]] std::vector<Type*> constraints() const;
    };

    /**
     * @struct StructTypeEntry
     * @brief Represents a field in a struct
     *
     * @typedef struct_type_entry_t
     * @brief Alias for StructTypeEntry
     */
    typedef struct StructTypeEntry {

        Type* m_type;       ///< The type of the field
        std::string name;   ///< The name of the field

    } struct_type_entry_t;

    /**
     * @class StructType
     * @brief Defines the semantic metadata for a packed set of variables
     *
     */
    class StructType : public Type {

        private:
            std::vector<struct_type_entry_t> m_entries;

        public:
            StructType(std::vector<struct_type_entry_t> entries);
            ~StructType() override;

            [[nodiscard]] std::string to_string() const final;

            [[nodiscard]] std::vector<struct_type_entry_t> entries() const;

    };

};

#endif //MCC_INTERNAL_LANGUAGE_TYPES_H