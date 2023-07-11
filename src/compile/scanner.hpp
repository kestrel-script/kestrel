#pragma once

#include <map>
#include <string>
#include <vector>

#include "token.hpp"

namespace kestrel {

class Scanner {

public:
  Scanner(String source) : source{source} {}

  std::vector<Token> scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }

    // tokens.push_back(TokenType::EndOfFile);
    tokens.emplace_back(TokenType::EndOfFile, "", "", line);
    return tokens;
  }

private:
  void scanToken() {
    char c = advance();
    switch (c) { // TODO use map?
    case '(':
      addToken(TokenType::LeftParenthesis);
      break;
    case ')':
      addToken(TokenType::RightParenthesis);
      break;
    case '{':
      addToken(TokenType::LeftBrace);
      break;
    case '}':
      addToken(TokenType::RightBrace);
      break;
    case ',':
      addToken(TokenType::Comma);
      break;
    case '.':
      addToken(TokenType::Dot);
      break;
    case '-':
      addToken(TokenType::Minus);
      break;
    case '+':
      addToken(TokenType::Plus);
      break;
    case ';':
      addToken(TokenType::Semicolon);
      break;
    case '*':
      addToken(TokenType::Star);
      break;
    case '!':
      addToken(match('=') ? TokenType::BangEqual : TokenType::Bang);
      break;
    case '=':
      addToken(match('=') ? TokenType::EqualEqual : TokenType::Equal);
      break;
    case '<':
      addToken(match('=') ? TokenType::LessThanEqual : TokenType::LessThan);
      break;
    case '>':
      addToken(match('=') ? TokenType::GreaterThanEqual
                          : TokenType::GreaterThan);
      break;

    case '/':
      if (match('/')) {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !isAtEnd())
          advance();
      } else {
        addToken(TokenType::Slash);
      }
      break;

    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace.
      break;

    case '\n':
      ++line;
      break;

    case '"':
      string();
      break;

    default:
      // error(line, "Unexpected character");

      if (isDigit(c)) {
        number();
      } else if (isAlpha(c)) {
        identifier();
      } else {
        // error(line, "Unexpected character.");
      }
      break;
    };
  }

  void identifier() {
    while (isAlphaNumeric(peek())) {
      advance();
    }

    String text = String{source.substr(start, current - start)};

    TokenType type = TokenType::Identifier;
    auto match = keywords.find(text);
    if (match != keywords.end()) {
      type = match->second;
    }
    addToken(type);
  }

  void number() {
    while (isDigit(peek())) {
      advance();
    }

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();
      while (isDigit(peek())) {
        advance();
      }
      addToken(TokenType::Double, source.substr(start, current - start));
      return;
    }
    addToken(TokenType::Integer, source.substr(start, current - start));
  }

  void string() {
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') {
        ++line;
      }
      advance();
    }

    if (isAtEnd()) {
      // error(line, "Unterminated string.");
      return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    String value{source.substr(start + 1, current - 2 - start)};
    addToken(TokenType::String, value);
  }

  bool match(char expected) {
    if (isAtEnd()) {
      return false;
    }

    if (source[current] != expected) {
      return false;
    }
    ++current;
    return true;
  }

  char peek() {
    if (isAtEnd()) {
      return '\0';
    }
    return source[current];
  }

  char peekNext() {
    if (current + 1 >= source.length()) {
      return '\0';
    }
    return source[current + 1];
  }

  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

  bool isDigit(char c) { return c >= '0' && c <= '9'; }

  bool isAtEnd() { return current >= source.length(); }

  char advance() { return source[current++]; }

  void addToken(TokenType type, String value = "") {
    String text{source.substr(start, current - start)};
    tokens.emplace_back(type, std::move(text), std::move(value), line);
  }
  // };

private:
  static const std::map<String, TokenType> keywords;

  String source;
  std::vector<Token> tokens;
  int start = 0;
  int current = 0;
  int line = 1;
};
// move to somewher else?
const std::map<String, TokenType> Scanner::keywords = {
    {"and", TokenType::And},         {"class", TokenType::Class},
    {"else", TokenType::Else},       {"false", TokenType::False},
    {"final", TokenType::While},     {"for", TokenType::For},
    {"get", TokenType::Get},         {"def", TokenType::Def},
    {"if", TokenType::If},           {"import", TokenType::Import},
    {"is", TokenType::Is},           {"nil", TokenType::Nil},
    {"nothing", TokenType::Nothing}, {"or", TokenType::Or},
    {"return", TokenType::Return},   {"set", TokenType::Set},
    {"super", TokenType::Super},     {"this", TokenType::This},
    {"true", TokenType::True},       {"let", TokenType::Var},
    {"while", TokenType::While},     {"module", TokenType::While},
};

} // namespace kestrel
