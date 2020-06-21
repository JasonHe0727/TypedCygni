#ifndef CYGNI_COMPILER_HPP
#define CYGNI_COMPILER_HPP
#include "Expression.hpp"
#include "Exception.hpp"
#include "Visitor.hpp"
#include <cstdint>
#include <vector>
#include <numeric>
#include <functional>

namespace cygni
{
	using Byte = uint8_t;

	class ByteCode
	{
	public:
		std::vector<Byte> bytes;
		ByteCode() = default;
		void Append(Byte byte);
		void AppendOp(OpCode op);
		void AppendTypeTag(TypeTag tag);
		void AppendU16Checked(size_t value, const std::function<CompilerException()>& exceptionHandler);
		void AppendU16Unchecked(size_t value);
		void WriteUShort(int index, int value);
		void AppendUInt(uint32_t value);
		void AppendTypeCode(TypeCode typeCode);
		void AppendString(const std::u32string& u32str);
		void AppendByteCode(const ByteCode& other);
		int Size() const;
	};

	using ConstantMap = std::unordered_map<ConstantKey, int>;

	class CompilerRule
	{
	public:
		std::vector<TypeCode> typeCodeList;
		OpCode op;
	};
	
	class CompilerRuleSet
	{
	public:
		std::unordered_map<ExpressionType, std::vector<CompilerRule>> table;

		void AddRule(ExpressionType nodeType, std::vector<TypeCode> typeCodeList, OpCode op);

		std::optional<OpCode> Match(ExpressionType nodeType, std::vector<TypeCode> typeCodeList);
	};
	class Compiler: public Visitor<void, const ConstantMap&, ByteCode&>
	{
	public:
		Project& project;
		CompilerRuleSet rules;
		Compiler(Project& project);
		ByteCode Compile();

		void VisitUnary(std::shared_ptr<UnaryExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitBinary(std::shared_ptr<BinaryExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitBlock(std::shared_ptr<BlockExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitConstant(std::shared_ptr<ConstantExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		//void CompileFieldDef(const FieldDef &field, ByteCode& byteCode);
		void VisitParameter(std::shared_ptr<ParameterExpression> parameter, const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitReturn(std::shared_ptr<ReturnExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitConditional(std::shared_ptr<ConditionalExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitDefault(std::shared_ptr<DefaultExpression> node, const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitInvocation(std::shared_ptr<InvocationExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitNewExpression(std::shared_ptr<NewExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitAssign(std::shared_ptr<BinaryExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void VisitWhile(std::shared_ptr<WhileExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);

		void CompileClassInfo(std::shared_ptr<ClassInfo> info, ByteCode& byteCode);
		void CompileModuleInfo(std::shared_ptr<ModuleInfo> info, ByteCode& byteCode);
		void CompileMethodDef(const MethodInfo &method, const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileMainFunction(const std::vector<std::shared_ptr<ModuleInfo>>& modules, ByteCode& byteCode);
		void CompileConstantPool(SourcePosition position, const ConstantMap& constantMap, ByteCode& byteCode);
		std::tuple<std::vector<std::shared_ptr<ClassInfo>>, std::vector<std::shared_ptr<ModuleInfo>>>
			CompileGlobalInformation(Project& project, ByteCode& byteCode);
		void ConvertExp(std::shared_ptr<UnaryExpression> node,ByteCode& byteCode);
		
	};
} // namespace cygni

#endif // CYGNI_COMPILER_HPP