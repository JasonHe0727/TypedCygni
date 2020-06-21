#include "opcode.h"

const char* opcode_info[][2] = {
		{ "PUSH_I32_0", "" },
	{ "PUSH_I32_1", "" },
	{ "PUSH_I32_1BYTE", "b" },
	{ "PUSH_I32_2BYTE", "u" },
	{ "PUSH_I64_0", "" },
	{ "PUSH_I64_1", "" },
	{ "PUSH_F64_0", "" },
	{ "PUSH_F64_1", "" },
	{ "PUSH_I32", "p" },
	{ "PUSH_I64", "p" },
	{ "PUSH_F32", "p" },
	{ "PUSH_F64", "p" },
	{ "PUSH_STRING", "p" },
	{ "PUSH_NULL", "" },
	{ "PUSH_LOCAL_I32", "u" },
	{ "PUSH_LOCAL_I64", "u" },
	{ "PUSH_LOCAL_F32", "u" },
	{ "PUSH_LOCAL_F64", "u" },
	{ "PUSH_LOCAL_OBJECT", "u" },
	{ "POP_LOCAL_I32", "u" },
	{ "POP_LOCAL_I64", "u" },
	{ "POP_LOCAL_F32", "u" },
	{ "POP_LOCAL_F64", "u" },
	{ "POP_LOCAL_OBJECT", "u" },
	{ "PUSH_STATIC_I32", "uu" },
	{ "PUSH_STATIC_I64", "uu" },
	{ "PUSH_STATIC_F32", "uu" },
	{ "PUSH_STATIC_F64", "uu" },
	{ "PUSH_STATIC_OBJECT", "uu" },
	{ "POP_STATIC_I32", "uu" },
	{ "POP_STATIC_I64", "uu" },
	{ "POP_STATIC_F32", "uu" },
	{ "POP_STATIC_F64", "uu" },
	{ "POP_STATIC_OBJECT", "uu" },
	{ "PUSH_ARRAY_I32", "" },
	{ "PUSH_ARRAY_I64", "" },
	{ "PUSH_ARRAY_F32", "" },
	{ "PUSH_ARRAY_F64", "" },
	{ "PUSH_ARRAY_OBJECT", "" },
	{ "POP_ARRAY_I32", "" },
	{ "POP_ARRAY_I64", "" },
	{ "POP_ARRAY_F32", "" },
	{ "POP_ARRAY_F64", "" },
	{ "POP_ARRAY_OBJECT", "" },
	{ "PUSH_FIELD_I32", "u" },
	{ "PUSH_FIELD_I64", "u" },
	{ "PUSH_FIELD_F32", "u" },
	{ "PUSH_FIELD_F64", "u" },
	{ "PUSH_FIELD_OBJECT", "u" },
	{ "POP_FIELD_I32", "u" },
	{ "POP_FIELD_I64", "u" },
	{ "POP_FIELD_F32", "u" },
	{ "POP_FIELD_F64", "u" },
	{ "POP_FIELD_OBJECT", "u" },
	{ "ADD_I32", "" },
	{ "ADD_I64", "" },
	{ "ADD_F32", "" },
	{ "ADD_F64", "" },
	{ "SUB_I32", "" },
	{ "SUB_I64", "" },
	{ "SUB_F32", "" },
	{ "SUB_F64", "" },
	{ "MUL_I32", "" },
	{ "MUL_I64", "" },
	{ "MUL_F32", "" },
	{ "MUL_F64", "" },
	{ "DIV_I32", "" },
	{ "DIV_I64", "" },
	{ "DIV_F32", "" },
	{ "DIV_F64", "" },
	{ "MOD_I32", "" },
	{ "MOD_I64", "" },
	{ "MOD_F32", "" },
	{ "MOD_F64", "" },
	{ "BIT_AND_I32", "" },
	{ "BIT_AND_I64", "" },
	{ "BIT_OR_I32", "" },
	{ "BIT_OR_I64", "" },
	{ "BIT_XOR_I32", "" },
	{ "BIT_XOR_I64", "" },
	{ "MINUS_I32", "" },
	{ "MINUS_I64", "" },
	{ "MINUS_F32", "" },
	{ "MINUS_F64", "" },
	{ "BIT_NOT_I32", "" },
	{ "BIT_NOT_I64", "" },
	{ "CAST_I32_TO_I64", "" },
	{ "CAST_I32_TO_F32", "" },
	{ "CAST_I32_TO_F64", "" },
	{ "CAST_I64_TO_I32", "" },
	{ "CAST_I64_TO_F32", "" },
	{ "CAST_I64_TO_F64", "" },
	{ "CAST_F32_TO_I32", "" },
	{ "CAST_F32_TO_I64", "" },
	{ "CAST_F32_TO_F64", "" },
	{ "CAST_F64_TO_I32", "" },
	{ "CAST_F64_TO_I64", "" },
	{ "CAST_F64_TO_F32", "" },
	{ "UP_CAST", "u" },
	{ "DOWN_CAST", "u" },
	{ "EQ_I32", "" },
	{ "EQ_I64", "" },
	{ "EQ_F32", "" },
	{ "EQ_F64", "" },
	{ "NE_I32", "" },
	{ "NE_I64", "" },
	{ "NE_F32", "" },
	{ "NE_F64", "" },
	{ "GT_I32", "" },
	{ "GT_I64", "" },
	{ "GT_F32", "" },
	{ "GT_F64", "" },
	{ "LT_I32", "" },
	{ "LT_I64", "" },
	{ "LT_F32", "" },
	{ "LT_F64", "" },
	{ "GE_I32", "" },
	{ "GE_I64", "" },
	{ "GE_F32", "" },
	{ "GE_F64", "" },
	{ "LE_I32", "" },
	{ "LE_I64", "" },
	{ "LE_F32", "" },
	{ "LE_F64", "" },
	{ "POP", "" },
	{ "DUPLICATE", "" },
	{ "DUPLICATE_OFFSET", "u" },
	{ "JUMP", "u" },
	{ "JUMP_IF_TRUE", "u" },
	{ "JUMP_IF_FALSE", "u" },
	{ "PUSH_FUNCTION", "uu" },
	{ "PUSH_METHOD", "uu" },
	{ "INVOKE", "" },
	{ "RETURN_I32", "" },
	{ "RETURN_I64", "" },
	{ "RETURN_F32", "" },
	{ "RETURN_F64", "" },
	{ "RETURN_OBJECT", "" },
	{ "NEW", "u" },
	{ "NEW_ARRAY", "bu" },
	{ "NEW_ARRAY_LITERAL_I32", "u" },
	{ "NEW_ARRAY_LITERAL_I64", "u" },
	{ "NEW_ARRAY_LITERAL_F32", "u" },
	{ "NEW_ARRAY_LITERAL_F64", "u" },
	{ "NEW_ARRAY_LITERAL_OBJECT", "u" },
	{ "ARRAY_LENGTH", "" },
	{ "SUPER", "" },
	{ "INSTANCE_OF", "u" },
	{ "THROW", "" },
	{ "RETHROW", "" },
	{ "GO_FINALLY", "u" },
	{ "FINALLY_END", "" }
};

const char* type_tag_to_string(TypeTag tag)
{
	switch (tag)
	{
	case TYPE_I32:
		return "TYPE_I32";
	case TYPE_I64:
		return "TYPE_I64";
	case TYPE_F32:
		return "TYPE_F32";
	case TYPE_F64:
		return "TYPE_F64";
	case TYPE_STRING:
		return "TYPE_STRING";
	case TYPE_OBJECT:
		return "TYPE_OBJECT";
	default:
		fprintf(stderr, __FUNCTION__);
		fprintf(stderr, "type tag enumeration\n");
		exit(-1);
	}
}