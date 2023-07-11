#pragma once

#include <string>

namespace kestrel {

using String = std::string;

enum class TokenType : unsigned char {
  // signs
  LeftParenthesis,  // (
  RightParenthesis, // )
  LeftBracket,      // [
  RightBracket,     // ]
  LeftBrace,        // {
  RightBrace,       // }
  Colon,
  Semicolon,
  Dot,
  Dotdot,    // TODO
  Dotdotdot, // TODO
  Comma,
  Star,
  Slash,
  Percent,
  Hash,
  Plus,
  Minus,
  Ltlt, // <<
  Gtgt, // >>
  Pipe, //
  Pipepipe,
  Caret,
  Amp,
  Ampamp,
  Bang,
  Tilde,
  Question,
  Equal, // sign?
  LessThan,
  GreaterThan,
  LessThanEqual,
  GreaterThanEqual,
  EqualEqual, // TODO
  BangEqual,

  Import,
  As,
  In,
  Is,

  // keywords;
  Break,
  Continue,
  Class,
  Construct, // TODO

  Final,
  For,
  Foreign,
  If,
  Else,

  Nil,
  Return,
  Static,
  Super,
  This,
  True,
  False,

  Def,
  Var,
  Val,

  Get,
  Set,
  Nothing,

  While,
  And,
  Or,
  Field,
  StaticField,
  Name,

  // Literals
  Identifier,
  String,
  Integer,
  Double,

  EndOfFile
};

enum class LiteralType {
  Integer,
  Double,
  kString,
};

union LiteralValue {
  int intValue;
  double doubleValue;
};

struct Literal {

  LiteralType type;
  LiteralValue value;
  std::string string;
};

class Token {
public:
  Token(TokenType type, String lexeme, String value, int line)
      : type{type}, lexeme{std::move(lexeme)}, value{std::move(lexeme)},
        line{line} {
    // Literal literal;
    switch (type) {
    case TokenType::Integer:
      literal.type = LiteralType::Integer;
      literal.value.intValue = std::stoi(value);
      break;
    case TokenType::Double:
      literal.type = LiteralType::Double;
      literal.value.doubleValue = std::stod(value);
      break;
    case TokenType::String:
      literal.type = LiteralType::kString;
      literal.string = value;
      break;
    default:
      // we don't care about the rest;
      break;
    }
  }

  operator String() const { return lexeme; }

  String toString() const {
    String literal_text;

    switch (type) {
    case (TokenType::Identifier):
      literal_text = lexeme;
      break;
    case (TokenType::String):
      literal_text = value;
      break;
    case (TokenType::Double): // TODO int? double:
      literal_text = std::to_string(literal.value.doubleValue);
      break;
    case (TokenType::True):
      literal_text = "true";
      break;
    case (TokenType::False):
      literal_text = "false";
      break;
    default:
      literal_text = "nil";
    }

    return std::to_string((int)type) + " " + lexeme + " " + literal_text;
  }

  // private:
  const TokenType type; // TODO use getter/setter;
  const String lexeme;
  const String value;
  const int line;
  Literal literal;
};

} // namespace kestrel