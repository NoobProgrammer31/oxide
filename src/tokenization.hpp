#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>

enum class TokenType {
  exit,
  inti,
  secol,
};

struct Token {
  TokenType type;
  std::optional<std::string> value;
};

class Tokenizer {
public:
  inline explicit Tokenizer(std::string src) : m_src(std::move(src)) 
  {

  }
  inline std::vector<Token> tokenize() {
    std::vector<Token> tokens;
    std::string buf;
    while (peak().has_value()) {
      if (std::isalpha(peak().value())) {
        buf.push_back(consume());
        while (peak().has_value() && std::isalnum(peak().value())) {
          buf.push_back(consume());
        }
        if (buf == "exit") {
          tokens.push_back({.type = TokenType::exit});
          buf.clear();
          continue;
        } else {
          std::cerr << "Messed up babe !\n";
          exit(EXIT_FAILURE);
        }
      } else if (std::isdigit(peak().value())) {
        buf.push_back(consume());
        while (peak().has_value() && std::isdigit(peak().value())) {
          buf.push_back(consume());
        }
        tokens.push_back({.type = TokenType::inti, .value = buf});
        buf.clear();
        continue;
      } else if (peak().value() == ';') {
        tokens.push_back({.type = TokenType::secol});
        continue;
      } else if (std::isspace(peak().value())) {
        continue;
      } else {
        std::cerr << "You Fucked up ! yet again :( \n";
        exit(EXIT_FAILURE);
      }
    }
    m_index =0;
    return tokens;
    }

private:
  [[nodiscard]] std::optional<char> peak(int ahead = 1) const {
    if (m_index + ahead >= m_src.length()) {
      return {};
    } else {
      return m_src.at(m_index);
    }
  }
  char consume() { return m_src.at(m_index++); }

  const std::string m_src;
  int m_index = 0;
};
