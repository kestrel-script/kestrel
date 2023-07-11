#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "log.hpp"
#include "token.hpp"
#include "value.hpp"

#include "statement.hpp"
#include "expression.hpp"

namespace kestrel {

class Compiler;

static LogLevel level = LogLevel::Debug;
static std::string tag = "stmt";


struct BlockStatement : Statement {
  BlockStatement(std::vector<std::shared_ptr<Statement>> statements)
      : statements{std::move(statements)} {}

  void evaluate(Compiler &compiler) override;
  void print() override { Log(level, tag) << "Block"; }

  const std::vector<std::shared_ptr<Statement>> statements;
};

struct ExpressionStatement : Statement {
  ExpressionStatement(std::shared_ptr<Expression> expression)
      : expression{std::move(expression)} {}

  void evaluate(Compiler &compiler) override;

  void print() override { Log(level, tag) << "Expression" << expression; }

  const std::shared_ptr<Expression> expression;
};

struct FunctionStatement : Statement {
  FunctionStatement(Token name, std::vector<Token> params,
                    std::vector<std::shared_ptr<Statement>> body)
      : name_{std::move(name)}, params_{std::move(params)}, body_{std::move(
                                                                body)} {
    if (body_.size() == 0) {
      throw - 1;
    }
  }

  void prepare(Compiler &compiler) override;
  void evaluate(Compiler &compiler) override;
  void print() override;

public:
  const Token &name() { return name_; }
  const std::vector<Token> &params() { return params_; }
  const std::vector<std::shared_ptr<Statement>> &body() { return body_; }

private:
  const Token name_;
  const std::vector<Token> params_;
  std::vector<std::shared_ptr<Statement>> body_;
};

struct IfStatement : Statement {
  IfStatement(std::shared_ptr<Expression> condition,
              std::shared_ptr<Statement> thenBranch,
              std::shared_ptr<Statement> elseBranch)
      : condition{std::move(condition)}, thenBranch{std::move(thenBranch)},
        elseBranch{std::move(elseBranch)} {}

  void evaluate(Compiler &compiler) override;
  void print() override { Log(level, tag) << "If "; }

  const std::shared_ptr<Expression> condition;
  const std::shared_ptr<Statement> thenBranch;
  const std::shared_ptr<Statement> elseBranch;
};

struct ReturnStatement : Statement {
  ReturnStatement(std::shared_ptr<Expression> value)
      : value{std::move(value)} {}

  void evaluate(Compiler &compiler) override;
  void print() override { Log(level, tag) << "Return"; }

  const std::shared_ptr<Expression> value;
};

struct VariableStatement : Statement {
  VariableStatement(Token name, std::shared_ptr<Expression> initializer)
      : name{std::move(name)}, initializer{std::move(initializer)} {}


  void prepare(Compiler& compiler) override {};

  void evaluate(Compiler &compiler) override;

  void print() override {
    Log(level, tag) << "Variable " << name.lexeme << " = " << initializer;
  }

  const Token name;
  const std::shared_ptr<Expression> initializer;
};

struct While : Statement {
  While(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> body)
      : condition{std::move(condition)}, body{std::move(body)} {}

  void print() override { Log(level, tag) << "While"; }

  const std::shared_ptr<Expression> condition;
  const std::shared_ptr<Statement> body;
};

struct Import : Statement {
  Import(Token name) : name{std::move(name)} {}

  void print() override { Log(level, tag) << "Import: " << name.toString(); }

  void evaluate(Compiler &compiler) override;

  const Token name;
};

struct ClassStatement : Statement {
  ClassStatement(Token name, std::shared_ptr<Variable> superclass,
                 std::vector<std::shared_ptr<FunctionStatement>> constructors,
                 std::vector<std::shared_ptr<FunctionStatement>> methods,
                 std::vector<std::shared_ptr<FunctionStatement>> getters,
                 std::vector<std::shared_ptr<FunctionStatement>> setters)
      : name{std::move(name)}, superclass{std::move(superclass)},
        constructors{std::move(constructors)}, methods{std::move(methods)},
        getters{std::move(getters)}, setters{std::move(setters)} {}

  void print() override {
    Log(level, tag) << "Class " << name.lexeme
                    << " constructors:" << constructors.size()
                    << " methods:" << methods.size()
                    << " getters: " << getters.size()
                    << " setters: " << setters.size();
  }

  const Token name;
  const std::shared_ptr<Variable> superclass;
  const std::vector<std::shared_ptr<FunctionStatement>> constructors;
  const std::vector<std::shared_ptr<FunctionStatement>> methods;
  const std::vector<std::shared_ptr<FunctionStatement>> getters;
  const std::vector<std::shared_ptr<FunctionStatement>> setters;
};

} // namespace kestrel