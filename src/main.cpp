#include <iostream>
#include "syntax/lexer.h"
#include "syntax/parser.h"
#include "vm/vm.h"

void CompileAndRun(const std::string& source) {
//  auto lexer = std::make_unique<Lexer>(source);
//  auto tokens = lexer->Lex();
//
//  auto parser = std::make_unique<Parser>(tokens);
//  auto expressions = parser->Parse();
//
  VM vm;
  vm.interpret(source);
}

int main() {
  auto source = R"(print 2 < 3;)";

  CompileAndRun(source);

  return 0;
}
