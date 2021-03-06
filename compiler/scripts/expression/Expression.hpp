#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "Enum.hpp"
#include <cstdint>
#include <list>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;

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

template <typename ExpReturnType, typename StatementReturnType,
          typename... ArgTypes>
class Visitor {
public:
  virtual ExpReturnType VisitExpression(Expression *node, ArgTypes... args) {
    switch (node->NodeType()) {
    case ExpressionType::ADD: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::SUBTRACT: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::MULTIPLY: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::DIVIDE: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::MODULO: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::GT: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::LT: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::GE: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::LE: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::EQ: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::NE: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::LOGICAL_AND: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::LOGICAL_OR: {
      return VisitBinary(dynamic_cast<BinaryExpression *>(node), args...);
    }
    case ExpressionType::INT: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::LONG: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::FLOAT: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::DOUBLE: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::BOOLEAN: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::CHAR: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::STRING: {
      return VisitConstant(dynamic_cast<ConstantExpression *>(node), args...);
    }
    case ExpressionType::PLUS: {
      return VisitUnary(dynamic_cast<UnaryExpression *>(node), args...);
    }
    case ExpressionType::MINUS: {
      return VisitUnary(dynamic_cast<UnaryExpression *>(node), args...);
    }
    case ExpressionType::LOGICAL_NOT: {
      return VisitUnary(dynamic_cast<UnaryExpression *>(node), args...);
    }
    case ExpressionType::INVOKE: {
      return VisitInvocation(dynamic_cast<InvocationExpression *>(node),
                             args...);
    }
    case ExpressionType::IDENTIFIER: {
      return VisitIdentifier(dynamic_cast<IdentifierExpression *>(node),
                             args...);
    }
    case ExpressionType::CONVERT: {
      return VisitConversion(dynamic_cast<ConversionExpression *>(node),
                             args...);
    }
    case ExpressionType::MEMBER: {
      return VisitMember(dynamic_cast<MemberExpression *>(node), args...);
    }
    case ExpressionType::NEW: {
      return VisitNew(dynamic_cast<NewExpression *>(node), args...);
    }
    default:
      throw Error(node->Pos(), "unsupported node type for visitor");
    }
  }
  virtual StatementReturnType VisitStatement(Statement *statement,
                                             ArgTypes... args) {
    switch (statement->GetStatementType()) {
    case StatementType::IF_THEN: {
      return VisitIfThen(dynamic_cast<IfThenStatement *>(node), args...);
    }
    case StatementType::IF_ELSE: {
      return VisitIfElse(dynamic_cast<IfElseStatement *>(node), args...);
    }
    case StatementType::WHILE: {
      return VisitWhile(dynamic_cast<WhileStatement *>(node), args...);
    }
    case StatementType::BLOCK: {
      return VisitBlock(dynamic_cast<BlockStatement *>(node), args...);
    }
    case StatementType::RETURN: {
      return VisitReturn(dynamic_cast<ReturnStatement *>(node), args...);
    }
    case StatementType::BREAK: {
      return VisitBreak(dynamic_cast<BreakStatement *>(node), args...);
    }
    case StatementType::ASSIGN: {
      return VisitAssign(dynamic_cast<AssignStatement *>(node), args...);
    }
    case StatementType::VAR_DECL: {
      return VisitVarDecl(dynamic_cast<VarDeclStatement *>(node), args...);
    }
    default: {
      throw Error(node->Pos(), "unsupported statement type for visitor");
    }
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

template <> class TypeChecker<TypePtr, void, ScopePtr> {
public:
  TypePtr VisitBinary(BinaryExpression *node, ScopePtr scope) override {}
  TypePtr VisitConstant(ConstantExpression *node, ScopePtr scope) override {}
  TypePtr VisitUnary(UnaryExpression *node, ScopePtr scope) override {}
  TypePtr VisitInvocation(InvocationExpression *node, ScopePtr scope) override {
  }
  TypePtr VisitIdentifier(IdentifierExpression *node, ScopePtr scope) override {
  }
  TypePtr VisitConversion(ConversionExpression *node, ScopePtr scope) override {
  }
  TypePtr VisitMember(MemberExpression *node, ScopePtr scope) override {}
  TypePtr VisitNew(NewExpression *node, ScopePtr scope) override {}
  void VisitIfThen(IfThenStatement *node, ScopePtr scope) override {}
  void VisitIfElse(IfElseStatement *node, ScopePtr scope) override {}
  void VisitWhile(WhileStatement *node, ScopePtr scope) override {}
  void VisitBlock(BlockStatement *node, ScopePtr scope) override {}
  void VisitReturn(ReturnStatement *node, ScopePtr scope) override {}
  void VisitBreak(BreakStatement *node, ScopePtr scope) override {}
  void VisitAssign(AssignStatement *node, ScopePtr scope) override {}
  void VisitVarDecl(VarDeclStatement *node, ScopePtr scope) override {}
};

template <> class AstJsonSerializer<json, json> {
public:
  json VisitBinary(BinaryExpression *node) override {}
  json VisitConstant(ConstantExpression *node) override {}
  json VisitUnary(UnaryExpression *node) override {}
  json VisitInvocation(InvocationExpression *node) override {}
  json VisitIdentifier(IdentifierExpression *node) override {}
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
};

#endif // EXPRESSION_HPP
