#include <iostream>
#include "lexer.h"
#include "parser.h"

void CompileAndRun(std::string source) {
  auto lexer = std::make_unique<Lexer>(source);
  auto tokens = lexer->Lex();

  auto parser = std::make_unique<Parser>(tokens);
  auto expressions = parser->Parse();

  auto compiler = std::make_unique<Compiler>();
}

int main() {
  auto source = R"(
10 + 2 + 3;
)";

  CompileAndRun(source);
  return 0;
}
