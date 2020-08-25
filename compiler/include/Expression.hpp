#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "Enum.hpp"
#include "Type.hpp"
#include <cstdint>
#include <list>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#define BINARY_TYPE_MATCH(NODE_TYPE, LEFT_TYPE, RIGHT_TYPE, RETURN_TYPE)       \
  if (node->NodeType() == ExpressionType::NODE_TYPE &&                         \
      left->Equals(Type::LEFT_TYPE()) && right->Equals(Type::RIGHT_TYPE())) {  \
    return Type::RETURN_TYPE();                                                \
  }

#define UNARY_TYPE_MATCH(NODE_TYPE, OPERAND_TYPE, RETURN_TYPE)                 \
  if (node->NodeType() == ExpressionType::NODE_TYPE &&                         \
      operand->Equals(Type::OPERAND_TYPE())) {                                 \
    return Type::RETURN_TYPE();                                                \
  }

using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;
using Byte = uint8_t;
using nlohmann::json;
using std::monostate;

class Position {
public:
  int line;
  int col;

  Position() : line{0}, col{0} {}
  Position(int line, int col) : line{line}, col{col} {}
};

class Statement {
public:
  virtual Position Pos() const = 0;
  virtual StatementType GetStatementType() const = 0;
};

class Expression : public Statement {
public:
  virtual ExpressionType NodeType() const = 0;
  StatementType GetStatementType() const override {
    return StatementType::EXPRESSION;
  }
};

class StatementList {
public:
  Statement *value;
  StatementList *next;
  StatementList(Statement *value, StatementList *next)
      : value{value}, next{next} {}
};

class BinaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  Expression *left;
  Expression *right;
  BinaryExpression(Position pos, ExpressionType nodeType, Expression *left,
                   Expression *right)
      : pos{pos}, nodeType{nodeType}, left{left}, right{right} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
};

class ConstantExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  string value;
  ConstantExpression(Position pos, ExpressionType nodeType, string value)
      : pos{pos}, nodeType{nodeType}, value{value} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
};

class UnaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  Expression *operand;
  UnaryExpression(Position pos, ExpressionType nodeType, Expression *operand)
      : pos{pos}, nodeType{nodeType}, operand{operand} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return nodeType; }
};

class InvocationExpression : public Expression {
public:
  Position pos;
  Expression *function;
  vector<Expression *> args;
  InvocationExpression(Position pos, Expression *function,
                       vector<Expression *> args)
      : pos{pos}, function{function}, args{args} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::INVOKE; }
};

class IdentifierExpression : public Expression {
public:
  Position pos;
  string identifier;
  IdentifierExpression(Position pos, string identifier)
      : pos{pos}, identifier{identifier} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override {
    return ExpressionType::IDENTIFIER;
  }
};

class ConversionExpression : public Expression {
public:
  Position pos;
  Expression *expression;
  TypePtr type;
  ConversionExpression(Position pos, Expression *expression, TypePtr type)
      : pos{pos}, expression{expression}, type{type} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::CONVERT; }
};

class MemberExpression : public Expression {
public:
  Position pos;
  Expression *object;
  string memberName;
  MemberExpression(Position pos, Expression *object, string memberName)
      : pos{pos}, object{object}, memberName{memberName} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::MEMBER; }
};

class NewExpression : public Expression {
public:
  Position pos;
  string className;
  string constructorName;
  vector<Expression *> args;
  NewExpression(Position pos, string className, string constructorName,
                vector<Expression *> args)
      : pos{pos}, className{className},
        constructorName{constructorName}, args{args} {}
  Position Pos() const override { return pos; }
  ExpressionType NodeType() const override { return ExpressionType::NEW; }
};

class BlockStatement : public Statement {
public:
  Position pos;
  vector<Statement *> statements;
  BlockStatement(Position pos, vector<Statement *> statements)
      : pos{pos}, statements{statements} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::BLOCK;
  }
};

class IfThenStatement : public Statement {
public:
  Position pos;
  Expression *condition;
  BlockStatement *ifTrue;
  IfThenStatement(Position pos, Expression *condition, BlockStatement *ifTrue)
      : pos{pos}, condition{condition}, ifTrue{ifTrue} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::IF_THEN;
  }
};

class IfElseStatement : public Statement {
public:
  Position pos;
  Expression *condition;
  BlockStatement *ifTrue;
  BlockStatement *ifFalse;
  IfElseStatement(Position pos, Expression *condition, BlockStatement *ifTrue,
                  BlockStatement *ifFalse)
      : pos{pos}, condition{condition}, ifTrue{ifTrue}, ifFalse{ifFalse} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::IF_ELSE;
  }
};

class WhileStatement : public Statement {
public:
  Position pos;
  Expression *condition;
  BlockStatement *body;
  WhileStatement(Position pos, Expression *condition, BlockStatement *body)
      : pos{pos}, condition{condition}, body{body} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::WHILE;
  }
};

class ReturnStatement : public Statement {
public:
  Position pos;
  Expression *value;
  ReturnStatement(Position pos, Expression *value) : pos{pos}, value{value} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::RETURN;
  }
};

class BreakStatement : public Statement {
public:
  Position pos;
  BreakStatement(Position pos) : pos{pos} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::BREAK;
  }
};

class AssignStatement : public Statement {
public:
  Position pos;
  AssignmentKind kind;
  Expression *left;
  Expression *value;
  AssignStatement(Position pos, AssignmentKind kind, Expression *left,
                  Expression *value)
      : pos{pos}, kind{kind}, left{left}, value{value} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::ASSIGN;
  }
};

class VarDeclStatement : public Statement {
public:
  Position pos;
  string identifier;
  optional<TypePtr> type;
  optional<Expression *> value;
  VarDeclStatement(Position pos, string identifier, optional<TypePtr> type,
                   optional<Expression *> value)
      : pos{pos}, identifier{identifier}, type{type}, value{value} {}
  Position Pos() const override { return pos; }
  StatementType GetStatementType() const override {
    return StatementType::VAR_DECL;
  }
};

class Parameter {
public:
  Position pos;
  string name;
  TypePtr type;
};

class MethodDeclStatement {
public:
  Position pos;
  string identifier;
  vector<Parameter *> parameters;
  BlockStatement *body;
  vector<VarDeclStatement *> localVariables;
};

class MemberDeclaration {
public:
  virtual Position Pos() const = 0;
  virtual MemberKind Kind() const = 0;
  virtual AccessModifier GetAccessModifier() const = 0;
};

class FunctionInfo;

class FieldMember : public MemberDeclaration {
public:
  Position pos;
  AccessModifier accessModifier;
  string name;
  Expression *initializer;
  int fieldIndex;
  weak_ptr<ClassInfo> classInfo;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::FIELD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

class MethodMember : public MemberDeclaration {
public:
  Position pos;
  AccessModifier accessModifier;
  bool isConstructor;
  bool isAbstract;
  bool isVirtual;
  bool isOverride;
  int methodIndex;
  FunctionType functionType; // 'this' is the first parameter
  ClassInfo *classInfo;
  FunctionInfo *functionInfo;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::METHOD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

class ClassInfo {
public:
  bool isAbstract;
  bool isInterface;
  string packageName;
  string name;
  vector<ClassInfo *> extendList;
  ClassInfo *superClass;
  vector<ClassInfo *> interfaceList;
  vector<FieldMember *> fields;
  vector<MethodMember *> methods;
  vector<FieldMember *> staticVariables;
  vector<MethodMember *> staticFunctions;
};

class FunctionInfo {
public:
  TypePtr type;
  string packageName;
  string name;
  vector<Parameter> parameters;
  shared_ptr<BlockStatement> block;
  vector<shared_ptr<VarDeclStatement>> localVariables;
};

class Import {
public:
  Position pos;
  string packageName;
};

class Rename {
public:
  Position pos;
  string packageName;
  string originalName;
  string newName;
};

class CodeFile {
public:
  string path;
  string packageName;
  vector<Import> importList;
  vector<Rename> renameList;
  vector<shared_ptr<ClassInfo>> classList;
};

class Program {
public:
  vector<shared_ptr<CodeFile>> codeFiles;
};

class ExpressionManager {
public:
  std::list<Expression *> list;
};

class Error {
public:
  Position pos;
  string message;
  Error(Position pos, string message) : pos{pos}, message{message} {}
};

template <typename ExpReturnType, typename StatementReturnType,
          typename... ArgTypes>
class Visitor {
public:
  virtual ExpReturnType VisitExpression(Expression *node, ArgTypes... args) {
    switch (node->NodeType()) {
    case ExpressionType::INT:
    case ExpressionType::LONG:
    case ExpressionType::FLOAT:
    case ExpressionType::DOUBLE:
    case ExpressionType::BOOLEAN:
    case ExpressionType::CHAR:
    case ExpressionType::STRING:
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    case ExpressionType::ADD:
    case ExpressionType::SUBTRACT:
    case ExpressionType::MULTIPLY:
    case ExpressionType::DIVIDE:
    case ExpressionType::MODULO:
    case ExpressionType::GT:
    case ExpressionType::LT:
    case ExpressionType::GE:
    case ExpressionType::LE:
    case ExpressionType::EQ:
    case ExpressionType::NE:
    case ExpressionType::LOGICAL_AND:
    case ExpressionType::LOGICAL_OR:
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    case ExpressionType::LOGICAL_NOT:
    case ExpressionType::PLUS:
    case ExpressionType::MINUS:
      return VisitUnary(dynamic_cast<UnaryExpression *>(node), args...);
    case ExpressionType::INVOKE:
      return VisitInvocation(dynamic_cast<InvocationExpression *>(node),
                             args...);
    case ExpressionType::IDENTIFIER:
      return VisitIdentifier(dynamic_cast<IdentifierExpression *>(node),
                             args...);
    case ExpressionType::CONVERT:
      return VisitConversion(dynamic_cast<ConversionExpression *>(node),
                             args...);
    case ExpressionType::MEMBER:
      return VisitMember(dynamic_cast<MemberExpression *>(node), args...);
    case ExpressionType::NEW:
      return VisitNew(dynamic_cast<NewExpression *>(node), args...);
    default:
      throw Error(node->Pos(), "unsupported node type for visitor");
    }
  }
  virtual StatementReturnType VisitStatement(Statement *statement,
                                             ArgTypes... args) {
    switch (statement->GetStatementType()) {
    case StatementType::EXPRESSION:
      return VisitExpStatement(dynamic_cast<Expression *>(statement), args...);
    case StatementType::IF_THEN:
      return VisitIfThen(dynamic_cast<IfThenStatement *>(statement), args...);
    case StatementType::IF_ELSE:
      return VisitIfElse(dynamic_cast<IfElseStatement *>(statement), args...);
    case StatementType::WHILE:
      return VisitWhile(dynamic_cast<WhileStatement *>(statement), args...);
    case StatementType::BLOCK:
      return VisitBlock(dynamic_cast<BlockStatement *>(statement), args...);
    case StatementType::RETURN:
      return VisitReturn(dynamic_cast<ReturnStatement *>(statement), args...);
    case StatementType::ASSIGN:
      return VisitAssign(dynamic_cast<AssignStatement *>(statement), args...);
    case StatementType::VAR_DECL:
      return VisitVarDecl(dynamic_cast<VarDeclStatement *>(statement), args...);
    default:
      throw Error(statement->Pos(), "unsupported statement type for visitor");
    }
  }
  virtual ExpReturnType VisitBinary(BinaryExpression *node,
                                    ArgTypes... args) = 0;
  virtual ExpReturnType VisitConstant(ConstantExpression *node,
                                      ArgTypes... args) = 0;
  virtual ExpReturnType VisitUnary(UnaryExpression *node, ArgTypes... args) = 0;
  virtual ExpReturnType VisitInvocation(InvocationExpression *node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitIdentifier(IdentifierExpression *node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitConversion(ConversionExpression *node,
                                        ArgTypes... args) = 0;
  virtual ExpReturnType VisitMember(MemberExpression *node,
                                    ArgTypes... args) = 0;
  virtual ExpReturnType VisitNew(NewExpression *node, ArgTypes... args) = 0;
  virtual StatementReturnType VisitIfThen(IfThenStatement *node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitIfElse(IfElseStatement *node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitWhile(WhileStatement *node,
                                         ArgTypes... args) = 0;
  virtual StatementReturnType VisitBlock(BlockStatement *node,
                                         ArgTypes... args) = 0;
  virtual StatementReturnType VisitReturn(ReturnStatement *node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitBreak(BreakStatement *node,
                                         ArgTypes... args) = 0;
  virtual StatementReturnType VisitAssign(AssignStatement *node,
                                          ArgTypes... args) = 0;
  virtual StatementReturnType VisitVarDecl(VarDeclStatement *node,
                                           ArgTypes... args) = 0;
  virtual StatementReturnType VisitExpStatement(Expression *node,
                                                ArgTypes... args) = 0;
};

template <typename T> class Scope {
public:
  unordered_map<string, T> table;
  weak_ptr<Scope<T>> parent;

  Scope() : parent() {}

  explicit Scope(shared_ptr<Scope<T>> parent) : parent(parent) {}

  optional<T> Get(string key) {
    if (table.count(key)) {
      return table[key];
    } else if (parent.expired()) {
      return make_optional<T>();
    } else {
      return parent.lock()->Get(key);
    }
  }

  /* true: success, false: failed */
  bool Put(string key, const T &value) {
    if (table.count(key)) {
      return false;
    } else {
      table.insert({key, value});
      return true;
    }
  }
};

template <typename T> using ScopePtr = shared_ptr<Scope<T>>;

// template <typename... ArgTypes>
// class NanoPass : public Visitor<ExpPtr, StatementPtr, ArgTypes...> {
//   virtual ExpPtr VisitConstant(shared_ptr<ConstantExpression> node,
//                                ArgTypes... args) {
//     return node;
//   }
//   virtual ExpPtr VisitBinary(shared_ptr<BinaryExpression> node,
//                              ArgTypes... args) {
//     auto left = VisitExpression(node->left, args...);
//     auto right = VisitExpression(node->right, args...);
//     return make_shared<BinaryExpression>(node->Pos(), node->NodeType(), left,
//                                          right);
//   }
//   virtual ExpPtr VisitUnary(shared_ptr<UnaryExpression> node,
//                             ArgTypes... args) {
//     auto operand = VisitExpression(node->operand, args...);
//     return make_shared<UnaryExpression>(node->Pos(), node->NodeType(),
//     operand);
//   }
//   virtual ExpPtr VisitInvocation(shared_ptr<InvocationExpression> node,
//                                  ArgTypes... args) {
//     auto function = VisitExpression(node->function, args...);
//     auto arguments =
//         Vec::Map<ExpPtr, ExpPtr>(node->args, [&](ExpPtr arg) -> ExpPtr {
//           return VisitExpression(arg, args...);
//         });
//     return make_shared<InvocationExpression>(node->Pos(), function,
//     arguments);
//   }
//   virtual ExpPtr VisitIdentifier(shared_ptr<IdentifierExpression> node,
//                                  ArgTypes... args) {
//     return node;
//   }
//   virtual ExpPtr VisitConversion(shared_ptr<ConversionExpression> node,
//                                  ArgTypes... args) {
//     auto expression = VisitExpression(node, args...);
//     return make_shared<ConversionExpression>(node->Pos(), expression,
//                                              node->type);
//   }
//   virtual ExpPtr VisitMember(shared_ptr<MemberExpression> node,
//                              ArgTypes... args) {
//     auto object = VisitExpression(node->object, args...);
//     return make_shared<MemberExpression>(node->Pos(), object, node->name);
//   }
//   virtual ExpPtr VisitNew(shared_ptr<NewExpression> node, ArgTypes... args) {
//     auto arguments =
//         Vec::Map<ExpPtr, ExpPtr>(node->args, [&](ExpPtr arg) -> ExpPtr {
//           return VisitExpression(arg, args...);
//         });
//     return make_shared<NewExpression>(node->Pos(), node->constructor,
//                                       arguments);
//   }
//   virtual StatementPtr VisitIfThen(shared_ptr<IfThenStatement> node,
//                                    ArgTypes... args) {
//     auto condition = VisitExpression(node->condition, args...);
//     auto ifTrue = VisitStatement(node->ifTrue, args...);
//     return make_shared<IfThenStatement>(node->Pos(), condition, ifTrue);
//   }
//   virtual StatementPtr VisitIfElse(shared_ptr<IfElseStatement> node,
//                                    ArgTypes... args) {
//     auto condition = VisitExpression(node->condition, args...);
//     auto ifTrue = VisitStatement(node->ifTrue, args...);
//     auto ifFalse = VisitStatement(node->ifFalse, args...);
//     return make_shared<IfElseStatement>(node->Pos(), condition, ifTrue,
//                                         ifFalse);
//   }
//   virtual StatementPtr VisitWhile(shared_ptr<WhileStatement> node,
//                                   ArgTypes... args) {
//     auto condition = VisitExpression(node->condition, args...);
//     auto body = VisitStatement(node->body, args...);
//     return make_shared<WhileStatement>(node->Pos(), condition, body);
//   }
//   virtual StatementPtr VisitBlock(shared_ptr<BlockStatement> node,
//                                   ArgTypes... args) {
//     auto statements = Vec::Map<StatementPtr, StatementPtr>(
//         node->statements, [&](StatementPtr x) -> StatementPtr {
//           return VisitStatement(x, args...);
//         });
//     return make_shared<BlockStatement>(node->Pos(), statements);
//   }
//   virtual StatementPtr VisitReturn(shared_ptr<ReturnStatement> node,
//                                    ArgTypes... args) {
//     auto value = VisitExpression(node->value, args...);
//     return make_shared<ReturnStatement>(node->Pos(), value);
//   }
//   virtual StatementPtr VisitAssign(shared_ptr<AssignStatement> node,
//                                    ArgTypes... args) {
//     auto left = VisitExpression(node->left, args...);
//     auto value = VisitExpression(node->value, args...);
//     return make_shared<AssignStatement>(node->Pos(), node->kind, left,
//     value);
//   }
//   virtual StatementPtr VisitVarDecl(shared_ptr<VarDeclStatement> node,
//                                     ArgTypes... args) {
//     if (node->value.has_value()) {
//       auto value = VisitExpression(node->value.value(), args...);
//       return make_shared<VarDeclStatement>(node->Pos(), node->identifier,
//                                            node->type, value);
//     } else {
//       return make_shared<VarDeclStatement>(node->Pos(), node->identifier,
//                                            node->type,
//                                            make_shared<ExpPtr>());
//     }
//   }
//   virtual StatementPtr VisitExpStatement(shared_ptr<Expression> node,
//                                          ArgTypes... args) {
//     return VisitExpression(node, args...);
//   }
// };

class ConstantMap {
public:
  int N;
  unordered_map<ExpressionType, unordered_map<string, int>> table;

  ConstantMap() : N{0}, table() {}

  void Add(ExpressionType type, string key) {
    if (table.count(type)) {
      if (table[type].count(key)) {
        /* pass */
      } else {
        table[type][key] = N;
        N = N + 1;
      }
    } else {
      table[type][key] = N;
      N++;
    }
  }

  int Get(ExpressionType type, string key) {
    if (table.count(type)) {
      if (table[type].count(key)) {
        return table[type][key];
      } else {
        table[type][key] = N;
        N = N + 1;
        return table[type][key];
      }
    } else {
      table[type][key] = N;
      N = N + 1;
      return table[type][key];
    }
  }

  void Clear() {
    table.clear();
    N = 0;
  }
};

class ByteCode {
public:
  vector<Byte> bytes;
  void Add(OpCode op) { bytes.push_back(static_cast<Byte>(op)); }
};

class TypeChecker : public Visitor<TypePtr, monostate, ScopePtr<TypePtr>> {
public:
  ByteCode bytes;
  ConstantMap constantMap;

  TypePtr VisitConstant(ConstantExpression *node,
                        ScopePtr<TypePtr> scope) override {
    constantMap.Get(node->NodeType(), node->value);
    switch (node->NodeType()) {
    case ExpressionType::INT: {
      bytes.Add(OpCode::PUSH_I32);
      return Type::Int();
    }
    case ExpressionType::BOOLEAN: {
      bytes.Add(OpCode::PUSH_I32);
      return Type::Boolean();
    }
    case ExpressionType::LONG: {
      bytes.Add(OpCode::PUSH_I64);
      return Type::Long();
    }
    case ExpressionType::DOUBLE: {
      bytes.Add(OpCode::PUSH_F64);
      return Type::Double();
    }
    case ExpressionType::CHAR: {
      bytes.Add(OpCode::PUSH_I32);
      return Type::Char();
    }
    case ExpressionType::STRING: {
      bytes.Add(OpCode::PUSH_STRING);
      return Type::String();
    }
    default:
      throw Error(node->Pos(), "error constant node type");
    }
  }

  TypePtr VisitBinary(BinaryExpression *node,
                      ScopePtr<TypePtr> scope) override {
    auto left = VisitExpression(node->left, scope);
    auto right = VisitExpression(node->right, scope);

    BINARY_TYPE_MATCH(ADD, Int, Int, Int);
    BINARY_TYPE_MATCH(ADD, Long, Long, Long);
    BINARY_TYPE_MATCH(ADD, Float, Float, Float);
    BINARY_TYPE_MATCH(ADD, Double, Double, Double);

    BINARY_TYPE_MATCH(SUBTRACT, Int, Int, Int);
    BINARY_TYPE_MATCH(SUBTRACT, Long, Long, Long);
    BINARY_TYPE_MATCH(SUBTRACT, Float, Float, Float);
    BINARY_TYPE_MATCH(SUBTRACT, Double, Double, Double);

    BINARY_TYPE_MATCH(MULTIPLY, Int, Int, Int);
    BINARY_TYPE_MATCH(MULTIPLY, Long, Long, Long);
    BINARY_TYPE_MATCH(MULTIPLY, Float, Float, Float);
    BINARY_TYPE_MATCH(MULTIPLY, Double, Double, Double);

    BINARY_TYPE_MATCH(DIVIDE, Int, Int, Int);
    BINARY_TYPE_MATCH(DIVIDE, Long, Long, Long);
    BINARY_TYPE_MATCH(DIVIDE, Float, Float, Float);
    BINARY_TYPE_MATCH(DIVIDE, Double, Double, Double);

    BINARY_TYPE_MATCH(MODULO, Int, Int, Int);
    BINARY_TYPE_MATCH(MODULO, Long, Long, Long);

    BINARY_TYPE_MATCH(GT, Int, Int, Boolean);
    BINARY_TYPE_MATCH(GT, Long, Long, Boolean);
    BINARY_TYPE_MATCH(GT, Float, Float, Boolean);
    BINARY_TYPE_MATCH(GT, Double, Double, Boolean);
    BINARY_TYPE_MATCH(GT, Char, Char, Boolean);

    BINARY_TYPE_MATCH(LT, Int, Int, Boolean);
    BINARY_TYPE_MATCH(LT, Long, Long, Boolean);
    BINARY_TYPE_MATCH(LT, Float, Float, Boolean);
    BINARY_TYPE_MATCH(LT, Double, Double, Boolean);
    BINARY_TYPE_MATCH(LT, Char, Char, Boolean);

    BINARY_TYPE_MATCH(GE, Int, Int, Boolean);
    BINARY_TYPE_MATCH(GE, Long, Long, Boolean);
    BINARY_TYPE_MATCH(GE, Float, Float, Boolean);
    BINARY_TYPE_MATCH(GE, Double, Double, Boolean);
    BINARY_TYPE_MATCH(GE, Char, Char, Boolean);

    BINARY_TYPE_MATCH(LE, Int, Int, Boolean);
    BINARY_TYPE_MATCH(LE, Long, Long, Boolean);
    BINARY_TYPE_MATCH(LE, Float, Float, Boolean);
    BINARY_TYPE_MATCH(LE, Double, Double, Boolean);
    BINARY_TYPE_MATCH(LE, Char, Char, Boolean);

    BINARY_TYPE_MATCH(EQ, Int, Int, Boolean);
    BINARY_TYPE_MATCH(EQ, Long, Long, Boolean);
    BINARY_TYPE_MATCH(EQ, Float, Float, Boolean);
    BINARY_TYPE_MATCH(EQ, Double, Double, Boolean);
    BINARY_TYPE_MATCH(EQ, Boolean, Boolean, Boolean);
    BINARY_TYPE_MATCH(EQ, Char, Char, Boolean);

    BINARY_TYPE_MATCH(NE, Int, Int, Boolean);
    BINARY_TYPE_MATCH(NE, Long, Long, Boolean);
    BINARY_TYPE_MATCH(NE, Float, Float, Boolean);
    BINARY_TYPE_MATCH(NE, Double, Double, Boolean);
    BINARY_TYPE_MATCH(NE, Boolean, Boolean, Boolean);
    BINARY_TYPE_MATCH(NE, Char, Char, Boolean);

    BINARY_TYPE_MATCH(LOGICAL_AND, Boolean, Boolean, Boolean);
    BINARY_TYPE_MATCH(LOGICAL_OR, Boolean, Boolean, Boolean);
    throw Error(node->Pos(), "binary operation type mismatch");
  }
  TypePtr VisitUnary(UnaryExpression *node, ScopePtr<TypePtr> scope) override {
    auto operand = VisitExpression(node->operand, scope);

    UNARY_TYPE_MATCH(PLUS, Int, Int);
    UNARY_TYPE_MATCH(PLUS, Long, Long);
    UNARY_TYPE_MATCH(PLUS, Float, Float);
    UNARY_TYPE_MATCH(PLUS, Double, Double);

    UNARY_TYPE_MATCH(MINUS, Int, Int);
    UNARY_TYPE_MATCH(MINUS, Long, Long);
    UNARY_TYPE_MATCH(MINUS, Float, Float);
    UNARY_TYPE_MATCH(MINUS, Double, Double);

    UNARY_TYPE_MATCH(LOGICAL_NOT, Boolean, Boolean);

    throw Error(node->Pos(), "unary operation type mismtach");
  }

  TypePtr VisitInvocation(InvocationExpression *node,
                          ScopePtr<TypePtr> scope) override {
    auto funcType = VisitExpression(node->function, scope);
    if (funcType->GetTypeCode() == TypeCode::METHOD) {
      return VisitMethodCall(node, static_pointer_cast<MethodType>(funcType),
                             scope);
    } else {
      return VisitFunctionCall(
          node, static_pointer_cast<FunctionType>(funcType), scope);
    }
  }
  TypePtr VisitMethodCall(InvocationExpression *node,
                          shared_ptr<MethodType> methodType,
                          ScopePtr<TypePtr> scope) {
    auto args = Vec::Map<Expression *, TypePtr>(
        node->args,
        [&](Expression *e) -> TypePtr { return VisitExpression(e, scope); });
    if (Vec::SequenceEqual(
            methodType->func->args, args,
            [](TypePtr x, TypePtr y) -> bool { return x->Equals(y); })) {
      return methodType->func->ret;
    } else {
      throw Error(node->Pos(), "method arguments' type mismtach");
    }
  }
  TypePtr VisitFunctionCall(InvocationExpression *node,
                            shared_ptr<FunctionType> function,
                            ScopePtr<TypePtr> scope) {
    auto args = Vec::Map<Expression *, TypePtr>(
        node->args,
        [&](Expression *e) -> TypePtr { return VisitExpression(e, scope); });
    if (function->GetTypeCode() == TypeCode::FUNCTION) {
      auto ft = static_pointer_cast<FunctionType>(function);
      if (Vec::SequenceEqual(ft->args, args, [](TypePtr x, TypePtr y) -> bool {
            return x->Equals(y);
          })) {
        return ft->ret;
      } else {
        throw Error(node->Pos(), "function arguments' type mismtach");
      }
    } else {
      throw Error(node->Pos(), "caller is not a function");
    }
  }
  TypePtr VisitIdentifier(IdentifierExpression *node,
                          ScopePtr<TypePtr> scope) override {
    if (auto value = scope->Get(node->identifier)) {
      return value.value();
    } else {
      throw Error(node->Pos(), "identifier not defined");
    }
  }
  TypePtr VisitConversion(ConversionExpression *node, ScopePtr<TypePtr> scope) {
    auto expression = VisitExpression(node, scope);
    // TO DO
    return node->type;
  }
  TypePtr VisitMember(MemberExpression *node,
                      ScopePtr<TypePtr> scope) override {
    auto object = VisitExpression(node->object, scope);
    if (object->GetTypeCode() == TypeCode::OBJECT) {
      auto objType = static_pointer_cast<ObjectType>(object);
      if (objType->fields.count(node->memberName)) {
        return objType->fields[node->memberName];
      } else {
        throw Error(node->Pos(), "member exp: field doesn't exist");
      }
    } else {
      throw Error(node->Pos(), "member exp: not object type");
    }
  }
  TypePtr VisitNew(NewExpression *node, ScopePtr<TypePtr> scope) override {}
  monostate VisitIfThen(IfThenStatement *node,
                        ScopePtr<TypePtr> scope) override {
    auto condition = VisitExpression(node->condition, scope);
    if (condition->Equals(Type::Boolean())) {
      auto subScope = make_shared<Scope<TypePtr>>(scope);
      VisitBlock(node->ifTrue, subScope);
    } else {
      throw Error(node->pos, "if then: condition type must be boolean");
    }
  }
  monostate VisitIfElse(IfElseStatement *node,
                        ScopePtr<TypePtr> scope) override {
    auto condition = VisitExpression(node->condition, scope);
    if (condition->Equals(Type::Boolean())) {
      auto subScope = make_shared<Scope<TypePtr>>(scope);
      VisitBlock(node->ifTrue, subScope);
      subScope = make_shared<Scope<TypePtr>>(scope);
      VisitBlock(node->ifFalse, subScope);
    } else {
      throw Error(node->pos, "if then: condition type must be boolean");
    }
  }
  monostate VisitWhile(WhileStatement *node, ScopePtr<TypePtr> scope) override {
    auto condition = VisitExpression(node->condition, scope);
    if (condition->Equals(Type::Boolean())) {
      auto subScope = make_shared<Scope<TypePtr>>(scope);
      VisitBlock(node->body, subScope);
    } else {
      throw Error(node->pos, "if then: condition type must be boolean");
    }
  }
  monostate VisitBlock(BlockStatement *node, ScopePtr<TypePtr> scope) override {
    for (auto statement : node->statements) {
      VisitStatement(statement, scope);
    }
    return monostate();
  }
  monostate VisitReturn(ReturnStatement *node,
                        ScopePtr<TypePtr> scope) override {
    VisitExpression(node->value, scope);
    return monostate();
  }
  monostate VisitAssign(AssignStatement *node,
                        ScopePtr<TypePtr> scope) override {
    return monostate();
  }
  monostate VisitVarDecl(VarDeclStatement *node,
                         ScopePtr<TypePtr> scope) override {
    return monostate();
  }
  monostate VisitExpStatement(Expression *node, ScopePtr<TypePtr> scope) {
    VisitExpression(node, scope);
    return monostate();
  }
};

class AstJsonSerializer : public Visitor<json, json> {
public:
  json VisitBinary(BinaryExpression *node) override {
    return unordered_map<string, json>(
        {{"Expression Type", Enum<ExpressionType>::ToString(node->NodeType())},
         {"Left", VisitExpression(node->left)},
         {"Right", VisitExpression(node->right)}});
  }
  json VisitConstant(ConstantExpression *node) override {
    return unordered_map<string, json>(
        {{"Expression Type", Enum<ExpressionType>::ToString(node->NodeType())},
         {"Value", node->value}});
  }
  json VisitUnary(UnaryExpression *node) override {
    return unordered_map<string, json>(
        {{"Expression Type", Enum<ExpressionType>::ToString(node->NodeType())},
         {"Operand", VisitExpression(node->operand)}});
  }
  json VisitInvocation(InvocationExpression *node) override {
    vector<json> args;
    for (auto arg : node->args) {
      args.push_back(VisitExpression(arg));
    }
    return unordered_map<string, json>(
        {{"Expression Type", Enum<ExpressionType>::ToString(node->NodeType())},
         {"Function", VisitExpression(node->function)},
         {"Arguments", args}});
  }
  json VisitIdentifier(IdentifierExpression *node) override {
    return unordered_map<string, json>(
        {{"Expression Type", Enum<ExpressionType>::ToString(node->NodeType())},
         {"Identifier", node->identifier}});
  }
  json VisitConversion(ConversionExpression *node) override {}
  json VisitMember(MemberExpression *node) override {}
  json VisitNew(NewExpression *node) override {}
  json VisitIfThen(IfThenStatement *node) override {}
  json VisitIfElse(IfElseStatement *node) override {}
  json VisitWhile(WhileStatement *node) override {}
  json VisitBlock(BlockStatement *node) override {}
  json VisitReturn(ReturnStatement *node) override {}
  json VisitBreak(BreakStatement *node) override {}
  json VisitAssign(AssignStatement *node) override {}
  json VisitVarDecl(VarDeclStatement *node) override {}
  json VisitExpStatement(Expression *node) override {}
};

class SyntaxError {
public:
  string path;
  Position pos;
  string message;
  SyntaxError() : path(), pos(), message() {}
  SyntaxError(string path, Position pos, string message)
      : path{path}, pos{pos}, message{message} {}
  string ToString() const {
    return path + ":" + std::to_string(pos.line + 1) + ":" +
           std::to_string(pos.col + 1) + ": " + message;
  }
};
#endif // EXPRESSION_HPP