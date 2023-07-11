#pragma once

#include <iostream>
#include <stdexcept>
#include "token.hpp"

namespace kestrel {

class RuntimeError: public std::runtime_error {
public:
  const Token& token;

  RuntimeError(const Token& token, String message)
    : std::runtime_error{message.data()}, token{token}
  {}
};

inline static void report(int line, String where,
                   String message) {
  std::cerr << "[line " << line << "] Error" << where << ": " 
    << message << "\n";
//   hadError = true;
}

inline void error(const Token& token, String message) {
  if (token.type == TokenType::EndOfFile) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at '" + token.lexeme + "'", message);
  }
}

inline void error(int line, String message) {
  report(line, "", message);
}

inline void runtimeError(const RuntimeError& error) {
  std::cerr << error.what() <<
      "\n[line " << error.token.line << "]\n";
//   hadRuntimeError = true;
}

}