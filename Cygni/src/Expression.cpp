#include "Expression.hpp"
#include "Exception.hpp"
#include <algorithm>


namespace cygni
{

	Expression::Expression(SourceLocation location, ExpressionType nodeType)
		: location{ location }, nodeType{ nodeType } {
		static int currentId = 0;
		this->id = currentId;
		currentId++;
		static std::shared_ptr<UnknownType> unknownType =
			std::make_shared<UnknownType>();
		this->type = unknownType;
	}

	ConstantExpression::ConstantExpression(SourceLocation location, TypePtr type,
		std::u32string constant)
		: Expression(location, ExpressionType::Constant), constant{ constant } {
		this->type = type;
	}

	BinaryExpression::BinaryExpression(SourceLocation location,
		ExpressionType nodeType, ExpPtr left,
		ExpPtr right)
		: Expression(location, nodeType), left{ left }, right{ right } {}

	UnaryExpression::UnaryExpression(SourceLocation location,
		ExpressionType nodeType, ExpPtr operand)
		: Expression(location, nodeType), operand{ operand } {}

	BlockExpression::BlockExpression(SourceLocation location, ExpList expressions)
		: Expression(location, ExpressionType::Block), expressions{ expressions } {}

	ConditionalExpression::ConditionalExpression(SourceLocation location,
		ExpPtr condition, ExpPtr ifTrue,
		ExpPtr ifFalse)
		: Expression(location, ExpressionType::Conditional), condition{ condition },
		ifTrue{ ifTrue }, ifFalse{ ifFalse } {}

	DefaultExpression::DefaultExpression(SourceLocation location, TypePtr type)
		: Expression(location, ExpressionType::Default)
	{
		this->type = type;
	}

	InvocationExpression::InvocationExpression(SourceLocation location,
		ExpPtr expression,
		std::vector<Argument> arguments)
		: Expression(location, ExpressionType::Invoke),
		expression{ expression }, arguments{ arguments } {}

	MethodCallExpression::MethodCallExpression(SourceLocation location,
		ExpPtr object,
		std::shared_ptr<MethodDef> method,
		ExpList arguments)
		: Expression(location, ExpressionType::MethodCall), object{ object },
		method{ method }, arguments{ arguments } {}

	NewExpression::NewExpression(SourceLocation location, std::u32string name,
		std::vector<Argument> arguments)
		: Expression(location, ExpressionType::New), name{ name }, arguments{
																	 arguments } {}

	ParameterExpression::ParameterExpression(SourceLocation location,
		std::u32string name, TypePtr type)
		: Expression(location, ExpressionType::Parameter), name{ name } {
		this->type = type;
	}

	VarDefExpression::VarDefExpression(
		SourceLocation location, std::shared_ptr<ParameterExpression> variable,
		TypePtr type, ExpPtr value)
		: Expression(location, ExpressionType::VariableDefinition),
		variable{ variable }, value{ value } {
		this->type = type;
	}

	FieldDef::FieldDef(SourceLocation location, AccessModifier modifier,
		bool isStatic, std::vector<AnnotationInfo> annotations,
		std::u32string name, TypePtr type, ExpPtr value)
		: location{ location }, modifier{ modifier }, isStatic{ isStatic },
		annotations{ annotations }, name{ name }, type{ type }, value{ value } {}

	MethodDef::MethodDef(
		SourceLocation location, AccessModifier modifier, bool isStatic,
		std::vector<AnnotationInfo> annotations, std::u32string name,
		std::vector<std::shared_ptr<ParameterExpression>> parameters,
		TypePtr returnType, ExpPtr body)
		: location{ location }, modifier{ modifier }, isStatic{ isStatic },
		annotations{ annotations }, name{ name }, parameters{ parameters },
		returnType{ returnType }, body{ body } {
		std::vector<TypePtr> parameterTypes(parameters.size());
		std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
			[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
		{
			return p->type;
		});
		this->signature = std::make_shared<FunctionType>(parameterTypes, returnType);
	}

	ClassInfo::ClassInfo(SourceLocation location, std::u32string name)
		: location{ location }, name{ name } {}

	ModuleInfo::ModuleInfo(SourceLocation location, std::u32string name)
		: location{ location }, name{ name } {}

	ReturnExpression::ReturnExpression(SourceLocation location, ExpPtr value)
		: Expression(location, ExpressionType::Return), value{ value } {}

	BreakExpression::BreakExpression(SourceLocation location)
		: Expression(location, ExpressionType::Break)
	{
	}

	WhileExpression::WhileExpression(SourceLocation location, ExpPtr condition,
		ExpPtr body)
		: Expression(location, ExpressionType::While), condition{ condition },
		body{ body } {}

	//Program::Program(std::shared_ptr<SourceDocument> document)
	//	: document{ document }
	//{
	//}

	//void Program::AddClass(std::shared_ptr<ClassInfo> info)
	//{
	//	classes.Add(info->name, info);
	//}

	//void Program::AddModule(std::shared_ptr<ModuleInfo> info)
	//{
	//	modules.Add(info->name, info);
	//}

	AnnotationInfo::AnnotationInfo(SourceLocation location, std::u32string name,
		std::vector<Argument> arguments)
		: location{ location }, name{ name }, arguments{ arguments } {}

	MemberAccessExpression::MemberAccessExpression(SourceLocation location,
		ExpPtr object,
		std::u32string field)
		: Expression(location, ExpressionType::MemberAccess), object{ object },
		field{ field } {}

	ConstructorInfo::ConstructorInfo(
		SourceLocation location, AccessModifier modifier,
		std::vector<AnnotationInfo> annotations, std::u32string name,
		std::vector<std::shared_ptr<ParameterExpression>> parameters,
		TypePtr returnType, ExpPtr body)
		: location{ location }, modifier{ modifier }, annotations{ annotations },
		name{ name }, parameters{ parameters }, returnType{ returnType }, body{ body } {
		std::vector<TypePtr> parameterTypes(parameters.size());
		std::transform(parameters.begin(), parameters.end(), parameterTypes.begin(),
			[](const std::shared_ptr<ParameterExpression> &p) -> TypePtr
		{
			return p->type;
		});
		this->signature = std::make_shared<FunctionType>(parameterTypes, returnType);
	}

	Argument::Argument(ExpPtr value) : name(), value{ value } {}

	Argument::Argument(std::optional<std::u32string> name, ExpPtr value)
		: name{ name }, value{ value } {}


	ParameterLocation::ParameterLocation() : type{ ParameterType::Unknown }, offset{ -1 }
	{
	}

	ParameterLocation::ParameterLocation(ParameterType type, int offset) : type{ type }, offset{ offset }
	{
	}

	Package::Package(std::u32string name) : name{ name }
	{
	}

	std::shared_ptr<Package> Package::FindPackage(const SourceLocation& location, const PackageRoute& route, int index)
	{
		if (index == route.size())
		{
			return shared_from_this();
		}
		else
		{
			const std::u32string& name = route.front();
			if (subPackages.find(name) != subPackages.end())
			{
				return subPackages[name]->FindPackage(location, route, index + 1);
			}
			else
			{
				throw SyntaxException(location, U"cannot find the pacakge");
			}
		}
	}

	std::shared_ptr<Package> Package::CreatePackage(const SourceLocation & location, const PackageRoute & route, int index)
	{
		if (index == route.size())
		{
			return shared_from_this();
		}
		else
		{
			const std::u32string& name = route.front();
			if (subPackages.find(name) != subPackages.end())
			{
				return subPackages[name]->CreatePackage(location, route, index + 1);
			}
			else
			{
				auto newPackage = std::make_shared<Package>(name);
				return newPackage->CreatePackage(location, route, index + 1);
			}
		}
	}

	Program::Program(std::shared_ptr<SourceDocument> document) :document{ document }
	{
	}

	TypeAlias::TypeAlias(PackageRoute route, std::u32string typeName, std::u32string alias)
		: route{ route }, typeName{ typeName }, alias{ alias }
	{
	}

} // namespace cygni
