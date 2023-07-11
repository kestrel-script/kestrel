#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "statements.hpp"
#include "token.hpp"

namespace kestrel {

class Parser {
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token> &tokens;
  int current = 0;

public:
  Parser(const std::vector<Token> &tokens) : tokens{tokens} {}

  std::vector<std::shared_ptr<Statement>> parse() {
    std::vector<std::shared_ptr<Statement>> statements;
    while (!isAtEnd()) {
      statements.push_back(declaration());
    }

    return statements;
  }

private:
  std::shared_ptr<Expression> expression() { return assignment(); }

  std::shared_ptr<Statement> declaration() {
    try {
      if (match(TokenType::Class))
        return classDeclaration();
      if (match(TokenType::Def))
        return function("function");
      if (match(TokenType::Var))
        return varDeclaration();
      if (match(TokenType::Import))
        return importDeclaration();
      // TODO val; import;

      // if (match(TokenType:))

      return statement();
    } catch (ParseError error) {
      Log(level, tag) << "parse error:" << error.what() ;
      synchronize();
      return nullptr;
    }
  }

  std::shared_ptr<Statement> classDeclaration() {
    Token name = consume(TokenType::Identifier, "Expect class name.");

    std::shared_ptr<Variable> superclass = nullptr;
    if (match(TokenType::Colon)) {
      consume(TokenType::Identifier, "Expect superclass name.");
      superclass = std::make_shared<Variable>(previous());
    }

    consume(TokenType::LeftBrace, "Expect '{' before class body.");

    std::vector<std::shared_ptr<FunctionStatement>> methods;
    std::vector<std::shared_ptr<FunctionStatement>> constructors;
    std::vector<std::shared_ptr<FunctionStatement>> getters;
    std::vector<std::shared_ptr<FunctionStatement>> setters;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
      // TODO check duplicate?
      if (check(TokenType::Identifier)) {
        // Log(level, tag) << "in constructor?" ;
        constructors.push_back(function("constructor"));
      }

      if (match(TokenType::Def)) {
        methods.push_back(function("method"));
      }

      if (match(TokenType::Get)) {
        getters.push_back(function("get"));
      }

      if (match(TokenType::Set)) {
        setters.push_back(function("set"));
      }
    }

    consume(TokenType::RightBrace, "Expect '}' after class body");

    if (constructors.empty()) {
        // TODO generate default constructor;
    }

    return std::make_shared<ClassStatement>(std::move(name), superclass,
                                   std::move(constructors),
                                   std::move(methods), std::move(getters),
                                   std::move(setters));
  }

  std::shared_ptr<Statement> statement() {
    if (match(TokenType::For)) {
      return forStatement();
    }
    if (match(TokenType::If)) {
      return ifStatement();
    }
    // if (match(PRINT)) return printStatement();
    if (match(TokenType::Return)) {
      return returnStatement();
    }
      
    if (match(TokenType::While)) {
      return whileStatement();
    }
      
    if (match(TokenType::LeftBrace)) {
      return std::make_shared<BlockStatement>(block());
    }
      

    return expressionStatement();
  }

  std::shared_ptr<Statement> forStatement() {
    consume(TokenType::LeftParenthesis, "Expect '(' after 'for'.");

    std::shared_ptr<Statement> initializer;
    if (match(TokenType::Semicolon)) {
      initializer = nullptr;
    } else if (match(TokenType::Var)) {
      initializer = varDeclaration();
    } else {
      initializer = expressionStatement();
    }

    std::shared_ptr<Expression> condition = nullptr;
    if (!check(TokenType::Semicolon)) {
      condition = expression();
    }
    consume(TokenType::Semicolon, "Expect ';' after loop condition.");

    std::shared_ptr<Expression> increment = nullptr;
    if (!check(TokenType::RightParenthesis)) {
      increment = expression();
    }
    consume(TokenType::RightParenthesis, "Expect ')' after for clauses.");
    std::shared_ptr<Statement> body = statement();

    if (increment != nullptr) {
      body = std::make_shared<BlockStatement>(std::vector<std::shared_ptr<Statement>>{
          body, std::make_shared<ExpressionStatement>(increment)});
        //TODO 
    }

    if (condition == nullptr) {
      condition = std::make_shared<LiteralExpression>(true);
    }
    body = std::make_shared<While>(condition, body);

    if (initializer != nullptr) {
      body = std::make_shared<BlockStatement>(
          std::vector<std::shared_ptr<Statement>>{initializer, body});
    }

    return body;
  }

  std::shared_ptr<Statement> ifStatement() {
    consume(TokenType::LeftParenthesis, "Expect '(' after 'if'.");
    std::shared_ptr<Expression> condition = expression();
    consume(TokenType::RightParenthesis, "Expect ')' after if condition.");

    std::shared_ptr<Statement> thenBranch = statement();
    std::shared_ptr<Statement> elseBranch = nullptr;
    if (match(TokenType::Else)) {
      elseBranch = statement();
    }

    return std::make_shared<IfStatement>(condition, thenBranch, elseBranch);
  }

  std::shared_ptr<Statement> returnStatement() {
    // Log(level, tag) << "returnStatement" ;
    Token keyword = previous();
    std::shared_ptr<Expression> value = nullptr;
    if (!check(TokenType::Semicolon)) {
      Log(level, tag) << "return !Semicolon" ;
      value = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after return value.");
    return std::make_shared<ReturnStatement>(value);
  }

  std::shared_ptr<Statement> importDeclaration() {
    Token name = consume(TokenType::Identifier, "Expect module name.");
    consume(TokenType::Semicolon, "Expect ';' after import declaration.");
    return std::make_shared<Import>(std::move(name));
  }

  std::shared_ptr<Statement> varDeclaration() {
    Token name = consume(TokenType::Identifier, "Expect variable name.");

    std::shared_ptr<Expression> initializer = nullptr;
    if (match(TokenType::Equal)) {
      initializer = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after variable declaration.");
    return std::make_shared<VariableStatement>(std::move(name), initializer);
  }

  std::shared_ptr<Statement> whileStatement() {
    consume(TokenType::LeftParenthesis, "Expect '(' after 'while'.");
    std::shared_ptr<Expression> condition = expression();
    consume(TokenType::RightParenthesis, "Expect ')' after condition.");
    std::shared_ptr<Statement> body = statement();

    return std::make_shared<While>(condition, body);
  }

  std::shared_ptr<Statement> expressionStatement() {
    std::shared_ptr<Expression> expr = expression();
    consume(TokenType::Semicolon, "Expect ';' after expression.");
    return std::make_shared<ExpressionStatement>(expr);
  }

  std::shared_ptr<FunctionStatement> function(String kind) {
    Token name = consume(TokenType::Identifier, "Expect " + kind + " name.");
    consume(TokenType::LeftParenthesis, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RightParenthesis)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Can't have more than 255 parameters.");
        }

        parameters.push_back(
            consume(TokenType::Identifier, "Expect parameter name."));
      } while (match(TokenType::Comma));
    }
    consume(TokenType::RightParenthesis, "Expect ')' after parameters.");

    consume(TokenType::LeftBrace, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Statement>> body = block();

    // Log(level, tag) << "body.size():" << body.size() ;
    return std::make_shared<FunctionStatement>(std::move(name), std::move(parameters), body);
  }

  std::vector<std::shared_ptr<Statement>> block() {
    std::vector<std::shared_ptr<Statement>> statements;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
      statements.push_back(declaration());
    }

    consume(TokenType::RightBrace, "Expect '}' after block.");
    return statements;
  }

  std::shared_ptr<Expression> assignment() {
    std::shared_ptr<Expression> expr = orExpression();

    if (match(TokenType::Equal)) {
      Token equals = previous();
      std::shared_ptr<Expression> value = assignment();

      if (Variable *e = dynamic_cast<Variable *>(expr.get())) {
        Token name = e->name;
        return std::make_shared<Assign>(std::move(name), value);
      } else if (Get *get = dynamic_cast<Get *>(expr.get())) {
        return std::make_shared<Set>(get->object, get->name, value);
      }

      error(std::move(equals), "Invalid assignment target.");
    }

    return expr;
  }

  std::shared_ptr<Expression> orExpression() {
    std::shared_ptr<Expression> expr = andExpression();

    while (match(TokenType::Or)) {
      Token op = previous();
      std::shared_ptr<Expression> right = andExpression();
      expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expression> andExpression() {
    std::shared_ptr<Expression> expr = equality();

    while (match(TokenType::And)) {
      Token op = previous();
      std::shared_ptr<Expression> right = equality();
      expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expression> equality() {
    std::shared_ptr<Expression> expr = comparison();

    // TODO use keywords?
    while (match(TokenType::BangEqual, TokenType::EqualEqual)) {
      Token op = previous();
      std::shared_ptr<Expression> right = comparison();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expression> comparison() {
    std::shared_ptr<Expression> expr = term();

    while (match(TokenType::GreaterThan, TokenType::GreaterThanEqual,
                 TokenType::LessThan, TokenType::LessThanEqual)) {
      Token op = previous();
      std::shared_ptr<Expression> right = term();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expression> term() {
    std::shared_ptr<Expression> expr = factor();

    while (match(TokenType::Minus, TokenType::Plus)) {
      Token op = previous();
      std::shared_ptr<Expression> right = factor();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expression> factor() {
    std::shared_ptr<Expression> expr = unary();

    while (match(TokenType::Slash, TokenType::Star)) {
      Token op = previous();
      std::shared_ptr<Expression> right = unary();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expression> unary() {
    // TODO bang to not?
    if (match(TokenType::Bang, TokenType::Minus)) {
      Token op = previous();
      std::shared_ptr<Expression> right = unary();
      return std::make_shared<Unary>(std::move(op), right);
    }

    return call();
  }

  std::shared_ptr<Expression> finishMethodCall(std::shared_ptr<Expression> callee, Token name) {
    std::vector<std::shared_ptr<Expression>> arguments;
    if (!check(TokenType::RightParenthesis)) {
      do {
        if (arguments.size() >= 255) {
          error(peek(), "Can't have more than 255 arguments.");
        }
        arguments.push_back(expression());
      } while (match(TokenType::Comma));
    }

    Token paren =
        consume(TokenType::RightParenthesis, "Expect ')' after arguments.");

    // TODO
    return std::make_shared<Dispatch>(callee, name, std::move(arguments));
  }

  std::shared_ptr<Expression> finishCall(std::shared_ptr<Expression> callee) {
    std::vector<std::shared_ptr<Expression>> arguments;
    if (!check(TokenType::RightParenthesis)) {
      do {
        if (arguments.size() >= 255) {
          error(peek(), "Can't have more than 255 arguments.");
        }
        arguments.push_back(expression());
      } while (match(TokenType::Comma));
    }

    Token paren =
        consume(TokenType::RightParenthesis, "Expect ')' after arguments.");

    return std::make_shared<Call>(callee, std::move(arguments));
  }

  std::shared_ptr<Expression> call() {
    std::shared_ptr<Expression> expr = primary();

    while (true) {
      if (match(TokenType::LeftParenthesis)) {
        expr = finishCall(expr);
      } else if (match(TokenType::Dot)) {
        Token name =
            consume(TokenType::Identifier, "Expect property name after '.'.");
        if (match(TokenType::LeftParenthesis)) {
          std::cout << "Method Call" << std::endl;
          expr = finishMethodCall(expr, name);
        } else {
          expr = std::make_shared<Get>(expr, std::move(name));
        }
        
      } else {
        break;
      }
    }

    return expr;
  }

  std::shared_ptr<Expression> primary() {
    if (match(TokenType::False))
      return std::make_shared<LiteralExpression>(false);
    if (match(TokenType::True))
      return std::make_shared<LiteralExpression>(true);
    if (match(TokenType::Nil)) // TODD
      return std::make_shared<LiteralExpression>();

    // TODO int and double?
    if (match(TokenType::Integer, TokenType::Double, TokenType::String)) {
      return std::make_shared<LiteralExpression>(previous().literal);
    }

    if (match(TokenType::Super)) {
      Token keyword = previous();
      consume(TokenType::Dot, "Expect '.' after 'super'.");
      Token method =
          consume(TokenType::Identifier, "Expect superclass method name.");
      return std::make_shared<Super>(std::move(keyword), std::move(method));
    }

    if (match(TokenType::This))
      return std::make_shared<This>(previous());

    if (match(TokenType::Var)) {
      return std::make_shared<Variable>(previous());
    }

    if (match(TokenType::Identifier)) {
      return std::make_shared<Variable>(previous());
    }

    if (match(TokenType::LeftParenthesis)) {
      Log(level, tag) << "LeftParenthesis" ;
      std::shared_ptr<Expression> expr = expression();
      consume(TokenType::RightParenthesis, "Expect ')' after expression.");
      return std::make_shared<Grouping>(expr);
    }
    if (match(TokenType::Identifier)) {
    //   TODO
    //   std::shared_ptr<Expression> expr = call();
    //   return std::make_shared<ExpressionStatement>(expr);
        return call(); //TODO
    }
    Log(level, tag) << "type:" << (int)peek().type ;
    throw error(peek(), "Expect expression.");
  }

  template<typename... Args>
  bool match(Args... type) {
    // assert((... && std::is_same_v<Args, TokenType>));
    auto values = {type...};
    for (auto&& x : values) {
      if (match(x)) {
        return true;
      }
	}
    return false;
  }
  //   template <class... T>
  bool match(TokenType type) {
    // Log(level, tag) << "checking type:" << (int)type ;

    if (check(type)) { // TODO handle array?
    //   Log(level, tag) << "type match" ;
      advance();
      return true;
    }
    // Log(level, tag) << "type not match" ;

    return false;
  }

  Token consume(TokenType type, String message) {
    if (check(type)) {
      return advance();
    }
    throw error(peek(), message);
  }

  bool check(TokenType type) { return isAtEnd() ? false : peek().type == type; }

  Token advance() {
    if (!isAtEnd()) {
      ++current;
    }
    return previous();
  }

  bool isAtEnd() { return peek().type == TokenType::EndOfFile; }

  Token peek() { return tokens.at(current); }

  Token previous() { return tokens.at(current - 1); }

  ParseError error(const Token &token, String message) {
    // ::error(token, message);
    return ParseError{message};
  }

  void synchronize() {
    Log(level, tag) << "synchronize" ;
    advance();

    while (!isAtEnd()) {
      if (previous().type == TokenType::Semicolon)
        return;

      switch (peek().type) {
        case TokenType::Class:
        case TokenType::Def:
        case TokenType::Var:
        case TokenType::For:
        case TokenType::If:
        case TokenType::While:
        case TokenType::Return:
            return;
        default: {
            return;
        }
      }
      advance();
    }
  }
};

}