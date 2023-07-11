#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>

namespace kestrel {

enum LogLevel {
  Trace,
  Debug,
  Info,
  Warning,
  Error,
  Fatal
};

using Writer = std::function<void(std::string&)>;

static const std::string black = "\033[30m";
static const std::string red = "\033[31m";
static const std::string green = "\033[32m";
static const std::string yellow = "\033[33m";
static const std::string blue = "\033[34m";
static const std::string magenta = "\033[35m";
static const std::string cyan = "\033[36m";
static const std::string white = "\033[37m";
static const std::string reset = "\033[0m";

static Writer sWriter = [](std::string& s) {
  std::cout << yellow << s << reset;
};

inline void setWriter(Writer writer) {
  sWriter = writer;
}

class Log {
public:
  Log(LogLevel level, bool newLine = true) : level(level) {
    this->level = level;
    this->newLine = newLine;
  }

  Log(LogLevel level, const std::string& tag, bool newLine = true) : level(level) {
    this->level = level;
    this->tag = tag;
    this->newLine = newLine;
  }

  void flush() {
    std::string content = stream.str();
    if (this->newLine) {
      content.append("\n");
    }
    stream.clear();
    sWriter(content);
  }

  ~Log() { flush(); }

private:
  std::string tag;
  bool newLine = false;
  std::stringstream stream;
  LogLevel level = LogLevel::Debug;

  template <typename T> friend Log &operator<<(Log &&log, T const &value);

  template <typename T> friend Log &operator<<(Log &log, T const &value);
};

template <typename T> Log &operator<<(Log &&log, T const &value) {
  log.stream << value;
  return log;
}

template <typename T> Log &operator<<(Log &log, T const &value) {
  log.stream << value;
  return log;
}

}