#ifndef VM_OP_CODE_H
#define VM_OP_CODE_H
#include <stdio.h>

enum OpCode
{
	PUSH_I32_0 = 0,
	PUSH_I32_1 = 1,
	PUSH_I32_1BYTE = 2,
	PUSH_I32_2BYTE = 3,
	PUSH_I64_0 = 4,
	PUSH_I64_1 = 5,
	PUSH_F64_0 = 6,
	PUSH_F64_1 = 7,
	PUSH_I32 = 8,
	PUSH_I64 = 9,
	PUSH_F32 = 10,
	PUSH_F64 = 11,
	PUSH_STRING = 12,
	PUSH_NULL = 13,
	PUSH_LOCAL_I32 = 14,
	PUSH_LOCAL_I64 = 15,
	PUSH_LOCAL_F32 = 16,
	PUSH_LOCAL_F64 = 17,
	PUSH_LOCAL_OBJECT = 18,
	POP_LOCAL_I32 = 19,
	POP_LOCAL_I64 = 20,
	POP_LOCAL_F32 = 21,
	POP_LOCAL_F64 = 22,
	POP_LOCAL_OBJECT = 23,
	PUSH_STATIC_I32 = 24,
	PUSH_STATIC_I64 = 25,
	PUSH_STATIC_F32 = 26,
	PUSH_STATIC_F64 = 27,
	PUSH_STATIC_OBJECT = 28,
	POP_STATIC_I32 = 29,
	POP_STATIC_I64 = 30,
	POP_STATIC_F32 = 31,
	POP_STATIC_F64 = 32,
	POP_STATIC_OBJECT = 33,
	PUSH_ARRAY_I32 = 34,
	PUSH_ARRAY_I64 = 35,
	PUSH_ARRAY_F32 = 36,
	PUSH_ARRAY_F64 = 37,
	PUSH_ARRAY_OBJECT = 38,
	POP_ARRAY_I32 = 39,
	POP_ARRAY_I64 = 40,
	POP_ARRAY_F32 = 41,
	POP_ARRAY_F64 = 42,
	POP_ARRAY_OBJECT = 43,
	PUSH_FIELD_I32 = 44,
	PUSH_FIELD_I64 = 45,
	PUSH_FIELD_F32 = 46,
	PUSH_FIELD_F64 = 47,
	PUSH_FIELD_OBJECT = 48,
	POP_FIELD_I32 = 49,
	POP_FIELD_I64 = 50,
	POP_FIELD_F32 = 51,
	POP_FIELD_F64 = 52,
	POP_FIELD_OBJECT = 53,
	ADD_I32 = 54,
	ADD_I64 = 55,
	ADD_F32 = 56,
	ADD_F64 = 57,
	SUB_I32 = 58,
	SUB_I64 = 59,
	SUB_F32 = 60,
	SUB_F64 = 61,
	MUL_I32 = 62,
	MUL_I64 = 63,
	MUL_F32 = 64,
	MUL_F64 = 65,
	DIV_I32 = 66,
	DIV_I64 = 67,
	DIV_F32 = 68,
	DIV_F64 = 69,
	MOD_I32 = 70,
	MOD_I64 = 71,
	MOD_F32 = 72,
	MOD_F64 = 73,
	BIT_AND_I32 = 74,
	BIT_AND_I64 = 75,
	BIT_OR_I32 = 76,
	BIT_OR_I64 = 77,
	BIT_XOR_I32 = 78,
	BIT_XOR_I64 = 79,
	MINUS_I32 = 80,
	MINUS_I64 = 81,
	MINUS_F32 = 82,
	MINUS_F64 = 83,
	BIT_NOT_I32 = 84,
	BIT_NOT_I64 = 85,
	CAST_I32_TO_I64 = 86,
	CAST_I32_TO_F32 = 87,
	CAST_I32_TO_F64 = 88,
	CAST_I64_TO_I32 = 89,
	CAST_I64_TO_F32 = 90,
	CAST_I64_TO_F64 = 91,
	CAST_F32_TO_I32 = 92,
	CAST_F32_TO_I64 = 93,
	CAST_F32_TO_F64 = 94,
	CAST_F64_TO_I32 = 95,
	CAST_F64_TO_I64 = 96,
	CAST_F64_TO_F32 = 97,
	UP_CAST = 98,
	DOWN_CAST = 99,
	EQ_I32 = 100,
	EQ_I64 = 101,
	EQ_F32 = 102,
	EQ_F64 = 103,
	NE_I32 = 104,
	NE_I64 = 105,
	NE_F32 = 106,
	NE_F64 = 107,
	GT_I32 = 108,
	GT_I64 = 109,
	GT_F32 = 110,
	GT_F64 = 111,
	LT_I32 = 112,
	LT_I64 = 113,
	LT_F32 = 114,
	LT_F64 = 115,
	GE_I32 = 116,
	GE_I64 = 117,
	GE_F32 = 118,
	GE_F64 = 119,
	LE_I32 = 120,
	LE_I64 = 121,
	LE_F32 = 122,
	LE_F64 = 123,
	POP = 124,
	DUPLICATE = 125,
	DUPLICATE_OFFSET = 126,
	JUMP = 127,
	JUMP_IF_TRUE = 128,
	JUMP_IF_FALSE = 129,
	PUSH_FUNCTION = 130,
	PUSH_METHOD = 131,
	INVOKE = 132,
	RETURN_I32 = 133,
	RETURN_I64 = 134,
	RETURN_F32 = 135,
	RETURN_F64 = 136,
	RETURN_OBJECT = 137,
	NEW = 138,
	NEW_ARRAY = 139,
	NEW_ARRAY_LITERAL_I32 = 140,
	NEW_ARRAY_LITERAL_I64 = 141,
	NEW_ARRAY_LITERAL_F32 = 142,
	NEW_ARRAY_LITERAL_F64 = 143,
	NEW_ARRAY_LITERAL_OBJECT = 144,
	ARRAY_LENGTH = 145,
	SUPER = 146,
	INSTANCE_OF = 147,
	THROW = 148,
	RETHROW = 149,
	GO_FINALLY = 150,
	FINALLY_END = 151
};

extern const char* opcode_info[][2];

typedef enum
{
	TYPE_I32,
	TYPE_I64,
	TYPE_F32,
	TYPE_F64,
	TYPE_STRING,
	TYPE_OBJECT
} TypeTag;


static const char* TypeTagToString(TypeTag tag)
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
	}
}
#endif // VM_OP_CODE_H