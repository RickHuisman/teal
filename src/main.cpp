#include <iostream>
#include "syntax/lexer.h"
#include "syntax/parser.h"

void CompileAndRun(std::string source) {
//  auto lexer = std::make_unique<Lexer>(source);
//  auto tokens = lexer->Lex();
//
//  auto parser = std::make_unique<Parser>(tokens);
//  auto expressions = parser->Parse();
//
  compile(source);
}

int main() {
  auto source = R"(
10 + 2 + 3;
)";

  CompileAndRun(source);

  return 0;
}
