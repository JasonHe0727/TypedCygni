#include "OpCode.h"
#include <iostream>

using namespace std;

wstring opcode_to_wstring(OpCode code)
{
	switch (code)
	{
		case OpCode::push_i32_1byte:
			return L"push_i32_1byte";
		case OpCode::push_i32_2byte:
			return L"push_i32_2byte";
		case OpCode::push_f64_0:
			return L"push_f64_0";
		case OpCode::push_f64_1:
			return L"push_f64_1";
		case OpCode::push_i32:
			return L"push_i32";
		case OpCode::push_f64:
			return L"push_f64";
		case OpCode::push_string:
			return L"push_string";
		case OpCode::push_null:
			return L"push_null";
		case OpCode::push_static_i32:
			return L"push_static_i32";
		case OpCode::push_static_f64:
			return L"push_static_f64";
		case OpCode::push_static_string:
			return L"push_static_string";
		case OpCode::push_stack_i32:
			return L"push_stack_i32";
		case OpCode::push_stack_f64:
			return L"push_stack_f64";
		case OpCode::push_stack_string:
			return L"push_stack_string";
		case OpCode::pop_static_i32:
			return L"pop_static_i32";
		case OpCode::pop_static_f64:
			return L"pop_static_f64";
		case OpCode::pop_static_string:
			return L"pop_static_string";
		case OpCode::pop_stack_i32:
			return L"pop_stack_i32";
		case OpCode::pop_stack_f64:
			return L"pop_stack_f64";
		case OpCode::pop_stack_string:
			return L"pop_stack_string";
		case OpCode::push_constant_i32:
			return L"push_constant_i32";
		case OpCode::push_constant_f64:
			return L"push_constant_f64";
		case OpCode::push_constant_string:
			return L"push_constant_string";
		case OpCode::store_constant_i32:
			return L"store_constant_i32";
		case OpCode::store_constant_f64:
			return L"store_constant_f64";
		case OpCode::store_constant_string:
			return L"store_constant_string";
		case OpCode::function_begin:
			return L"function_begin";
		case OpCode::function_end:
			return L"function_end";
		case OpCode::add_i32:
			return L"add_i32";
		case OpCode::sub_i32:
			return L"sub_i32";
		case OpCode::mul_i32:
			return L"mul_i32";
		case OpCode::div_i32:
			return L"div_i32";
		case OpCode::mod_i32:
			return L"mod_i32";
		case OpCode::add_f64:
			return L"add_f64";
		case OpCode::sub_f64:
			return L"sub_f64";
		case OpCode::mul_f64:
			return L"mul_f64";
		case OpCode::div_f64:
			return L"div_f64";
		case OpCode::mod_f64:
			return L"mod_f64";
		case OpCode::gt_i32:
			return L"gt_i32";
		case OpCode::lt_i32:
			return L"lt_i32";
		case OpCode::ge_i32:
			return L"ge_i32";
		case OpCode::le_i32:
			return L"le_i32";
		case OpCode::eq_i32:
			return L"eq_i32";
		case OpCode::ne_i32:
			return L"ne_i32";
		case OpCode::gt_f64:
			return L"gt_f64";
		case OpCode::lt_f64:
			return L"lt_f64";
		case OpCode::ge_f64:
			return L"ge_f64";
		case OpCode::le_f64:
			return L"le_f64";
		case OpCode::eq_f64:
			return L"eq_f64";
		case OpCode::ne_f64:
			return L"ne_f64";
		case OpCode::logical_and:
			return L"logical_and";
		case OpCode::logical_or:
			return L"logical_or";
		case OpCode::logical_not:
			return L"logical_not";
		case OpCode::minus_i32:
			return L"minus_i32";
		case OpCode::minus_f64:
			return L"minus_f64";
		case OpCode::cast_int32_to_float64:
			return L"cast_int32_to_float64";
		case OpCode::cast_float64_to_int32:
			return L"cast_float64_to_int32";
		case OpCode::jump:
			return L"jump";
		case OpCode::jump_if_true:
			return L"jump_if_true";
		case OpCode::jump_if_false:
			return L"jump_if_false";
		case OpCode::return_i32:
			return L"return_i32";
		case OpCode::return_f64:
			return L"return_f64";
		case OpCode::push_function:
			return L"push_function";
		case OpCode::invoke:
			return L"invoke";
		case OpCode::push_array_i32:
			return L"push_array_i32";
		case OpCode::push_array_f64:
			return L"push_array_f64";
		case OpCode::push_array_object:
			return L"push_array_object";
		case OpCode::pop_array_i32:
			return L"pop_array_i32";
		case OpCode::pop_array_f64:
			return L"pop_array_f64";
		case OpCode::pop_array_object:
			return L"pop_array_object";
		case OpCode::new_array:
			return L"new_array";
		case OpCode::new_array_literal_i32:
			return L"new_array_literal_i32";
		case OpCode::new_array_literal_f64:
			return L"new_array_literal_f64";
		case OpCode::new_array_literal_object:
			return L"new_array_literal_object";
		case OpCode::constant_i32:
			return L"constant_i32";
		case OpCode::constant_i64:
			return L"constant_i64";
		case OpCode::constant_f64:
			return L"constant_f64";
		case OpCode::constant_string:
			return L"constant_string";
		default:
			throw L"error opcode";
	}
}

i32 OperandSize(OpCode op)
{
	switch (op)
	{
		case OpCode::push_f64_0:
		case OpCode::push_f64_1:
		case OpCode::function_begin:
		case OpCode::function_end:
		case OpCode::add_i32:
		case OpCode::sub_i32:
		case OpCode::mul_i32:
		case OpCode::div_i32:
		case OpCode::mod_i32:

		case OpCode::add_f64:
		case OpCode::sub_f64:
		case OpCode::mul_f64:
		case OpCode::div_f64:
		case OpCode::mod_f64:

		case OpCode::gt_i32:
		case OpCode::lt_i32:
		case OpCode::ge_i32:
		case OpCode::le_i32:
		case OpCode::eq_i32:
		case OpCode::ne_i32:

		case OpCode::gt_f64:
		case OpCode::lt_f64:
		case OpCode::ge_f64:
		case OpCode::le_f64:
		case OpCode::eq_f64:
		case OpCode::ne_f64:

		case OpCode::minus_i32:

		case OpCode::minus_f64:

		case OpCode::cast_int32_to_float64:
		case OpCode::cast_float64_to_int32:

		case OpCode::return_i32:
		case OpCode::return_f64:
			return 0;
		case OpCode::push_i32_1byte:
			return 1;
		case OpCode::push_i32_2byte:
		case OpCode::push_i32:
		case OpCode::push_f64:
		case OpCode::push_string:
		case OpCode::push_function:
		case OpCode::invoke:

		case OpCode::push_static_i32:
		case OpCode::push_static_f64:
		case OpCode::push_static_string:

		case OpCode::push_stack_i32:
		case OpCode::push_stack_f64:
		case OpCode::push_stack_string:

		case OpCode::pop_static_i32:
		case OpCode::pop_static_f64:
		case OpCode::pop_static_string:

		case OpCode::pop_stack_i32:
		case OpCode::pop_stack_f64:
		case OpCode::pop_stack_string:

		case OpCode::push_constant_i32:
		case OpCode::push_constant_f64:
		case OpCode::push_constant_string:

		case OpCode::jump:
		case OpCode::jump_if_true:
		case OpCode::jump_if_false:
			return 2;

		default:
			wcout << L"not supported Operand Size: " << endl;
			wcout << opcode_to_wstring(op) << endl;
			throw L"not supported Operand Size: ";
	}
}