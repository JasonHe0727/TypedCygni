#include "TypeTag.h"

wstring type_tag_to_wstring(TypeTag tag)
{
	switch (tag)
	{
		case TypeTag::Unknown:
			return L"Unknown";
		case TypeTag::Int:
			return L"Int";
		case TypeTag::Long:
			return L"Long";
		case TypeTag::Float:
			return L"Float";
		case TypeTag::Double:
			return L"Double";
		case TypeTag::Boolean:
			return L"Boolean";
		case TypeTag::Char:
			return L"Char";
		case TypeTag::String:
			return L"String";
		case TypeTag::Unit:
			return L"Unit";
		case TypeTag::Null:
			return L"Null";
		case TypeTag::Array:
			return L"Array";
		case TypeTag::Base:
			return L"Base";
		case TypeTag::Inherited:
			return L"Inherited";
		case TypeTag::Function:
			return L"Function";
		case TypeTag::Any:
			return L"Any";
	}
}
