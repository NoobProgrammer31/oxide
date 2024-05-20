#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
using namespace std;

enum class TokenType {
  exit,
  inti,
  secol,
};

struct Token {
  TokenType type;
  optional<string> value;
};

vector<Token> tokenize(const string &str) {
  vector<Token> tokens;
  string buf;
  for (int i = 0; i < str.length(); i++) {
    char c = str.at(i);
    if (isalpha(c)) {
      buf.push_back(c);
      i++;
      while (isalnum(str.at(i))) {
        buf.push_back(str.at(i));
        i++;
      }
      i--;

      if (buf == "exit") {
        tokens.push_back({.type = TokenType::exit});
        buf.clear();
        continue;
      } else {
        cerr << "Messed up babe !\n";
        exit(EXIT_FAILURE);
      }
    } else if (isdigit(c)) {
      buf.push_back(c);
      i++;
      while (isdigit(str.at(i))) {
        buf.push_back(str.at(i));
        i++;
      }
      i--;
      tokens.push_back({.type = TokenType::inti, .value = buf});
      buf.clear();
    } else if (c == ';') {
      tokens.push_back({.type = TokenType::secol});
    } else if (isspace(c)) {
      continue;
    } else {
      cerr << "You Fucked up ! yet again :( \n";
      exit(EXIT_FAILURE);
    }
  }
  return tokens;
}

string tokens_to_asm(const vector<Token> &tokens) {
  stringstream output;
  output << "global _start\n_start:\n";
  for (int i = 0; i < tokens.size(); i++) {
    const Token &token = tokens.at(i);
    if (token.type == TokenType::exit) {
      if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::inti) {
        if (i + 2 < tokens.size() &&
            tokens.at(i + 2).type == TokenType::secol) {
          output << "    mov rax, 60\n";
          output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
          output << "    syscall";
        }
      }
    }
  }
  return output.str();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Incorrect Usage. Correct usage is...\n";
    cerr << "hydro <input.hy>\n";
    return EXIT_FAILURE;
  }
  string contents;
  {
    stringstream contents_stream;
    fstream input(argv[1], ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
  }
  vector<Token> tokens = tokenize(contents);
  {
    fstream file("out.asm", ios::out);
    file << tokens_to_asm(tokens);
  }
  system("nasm -felf64 out.asm");
  system("ld -o out out.o");
  return EXIT_SUCCESS;
}
