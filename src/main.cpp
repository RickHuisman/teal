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
//  auto source = R"(print 10;)";
//
//  CompileAndRun(source);

  std::vector<Value> values;

  values.push_back(NumberValue(10.0));

  auto value = values[0];

  auto *number = dynamic_cast<NumberValue*>(&value);

  std::cout << number->number << std::endl;

  return 0;
}
