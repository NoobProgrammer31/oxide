#include "tokenization.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
using namespace std;

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
  Tokenizer tokenizer(std::move(contents));
  vector<Token> tokens = tokenizer.tokenize();
  {
    fstream file("out.asm", ios::out);
    file << tokens_to_asm(tokens);
  }
  system("nasm -felf64 out.asm");
  system("ld -o out out.o");
  return EXIT_SUCCESS;
}
