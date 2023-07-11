#pragma once

#include "token.hpp"
#include "value.hpp"

#include <memory>
#include <vector>

#include <iostream>

namespace kestrel {

struct Assign;
struct Binary;
struct Call;
struct Get;
struct Dispatch;
struct Grouping;
struct LiteralExpression;
struct Logical;
struct Set;
struct Super;
struct This;
struct Unary;
struct Variable;

class Compiler;

enum class ExpressionType {
  Assign,
  Binary,
  Call,
  Get,
  Grouping,
  Literal,
  Logical,
  Set,
  Super,
  This,
  Unary,
  Variable,
};

struct Expression {
  virtual void eval(Compiler &compiler) {
    throw std::runtime_error("Expression::eval not implemented");
  }

  virtual ExpressionType type() = 0; 
};

struct Assign : Expression {
  Assign(Token name, std::shared_ptr<Expression> value)
      : name{std::move(name)}, value{std::move(value)} {}

  ExpressionType type() override { return ExpressionType::Assign;}
  void eval(Compiler &compiler) override;

  const Token name;
  const std::shared_ptr<Expression> value;
};

struct Binary : Expression {
  Binary(std::shared_ptr<Expression> left, Token op,
         std::shared_ptr<Expression> right)
      : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} {}

  ExpressionType type() override { return ExpressionType::Binary;}
  void eval(Compiler &compiler) override;

  const Token op;
  const std::shared_ptr<Expression> left;
  const std::shared_ptr<Expression> right;
};

struct Call : Expression {
  Call(std::shared_ptr<Expression> callee,
       std::vector<std::shared_ptr<Expression>> arguments)
      : callee{std::move(callee)}, arguments{std::move(arguments)} {}

  ExpressionType type() override { return ExpressionType::Call;}
  void eval(Compiler &compiler) override;

  const std::shared_ptr<Expression> callee;
  const std::vector<std::shared_ptr<Expression>> arguments;
};

struct Get : Expression {
  Get(std::shared_ptr<Expression> object, Token name)
      : object{std::move(object)}, name{std::move(name)} {
        std::cout << "Get Expression" << std::endl;
      }

  ExpressionType type() override { return ExpressionType::Get;}
  void eval(Compiler &compiler) override;

  const Token name;
  const std::shared_ptr<Expression> object;
};

struct Dispatch: Expression {
  Dispatch(std::shared_ptr<Expression> object, Token name,
       std::vector<std::shared_ptr<Expression>> arguments)
      : object{std::move(object)}, name(std::move(name)), 
      arguments{std::move(arguments)} {}

  ExpressionType type() override { return ExpressionType::Call;}
  void eval(Compiler &compiler) override;

  const Token name;
  const std::shared_ptr<Expression> object;
  const std::vector<std::shared_ptr<Expression>> arguments;
};

struct Grouping : Expression {
  Grouping(std::shared_ptr<Expression> expression)
      : expression{std::move(expression)} {}

  ExpressionType type() override { return ExpressionType::Grouping;}
  void eval(Compiler &compiler) override;

  const std::shared_ptr<Expression> expression;
};

struct LiteralExpression : Expression {
  LiteralExpression(std::shared_ptr<Value> value) : value{std::move(value)} {}

  LiteralExpression(double d) { value = std::make_shared<Value>(d); }

  LiteralExpression(Literal lit) {
    switch (lit.type) {
      case LiteralType::Double: {
        value = std::make_shared<Value>(lit.value.doubleValue); // TODO
        break;
      }
      case LiteralType::Integer: {
        value = std::make_shared<Value>(lit.value.intValue); // TODO
        break;
      }
      case LiteralType::kString: {
        value = std::make_shared<Value>(lit.string); // TODO
        break;
      }
    }

  } // TODO

  LiteralExpression() {}

  ExpressionType type() override { return ExpressionType::Literal;}
  void eval(Compiler &compiler) override;

  std::shared_ptr<Value> value;
};

struct Logical : Expression {
  Logical(std::shared_ptr<Expression> left, Token op,
          std::shared_ptr<Expression> right)
      : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} {}

  ExpressionType type() override { return ExpressionType::Logical;}
  void eval(Compiler &compiler) override;

  const Token op;
  const std::shared_ptr<Expression> left;
  const std::shared_ptr<Expression> right;
};

struct Set : Expression {
  Set(std::shared_ptr<Expression> object, Token name,
      std::shared_ptr<Expression> value)
      : object{std::move(object)}, name{std::move(name)}, value{std::move(
                                                              value)} {}

  ExpressionType type() override { return ExpressionType::Set;}
  void eval(Compiler &compiler) override;

  const std::shared_ptr<Expression> object;
  const Token name;
  const std::shared_ptr<Expression> value;
};

struct Super : Expression {
  Super(Token keyword, Token method)
      : keyword{std::move(keyword)}, method{std::move(method)} {}

  ExpressionType type() override { return ExpressionType::Super;}
  void eval(Compiler &compiler) override;

  const Token keyword;
  const Token method;
};

struct This : Expression {
  This(Token keyword) : keyword{std::move(keyword)} {}

  ExpressionType type() override { return ExpressionType::This;}
  void eval(Compiler &compiler) override;

  const Token keyword;
};

struct Unary : Expression {
  Unary(Token op, std::shared_ptr<Expression> right)
      : op{std::move(op)}, right{std::move(right)} {}

  ExpressionType type() override { return ExpressionType::Unary;}
  void eval(Compiler &compiler) override;

  const Token op;
  const std::shared_ptr<Expression> right;
};

struct Variable : Expression {
  Variable(Token name) : name{std::move(name)} {}

  ExpressionType type() override { return ExpressionType::Variable;}
  void eval(Compiler &compiler) override;

  const Token name;
};

} // namespace kestrel
