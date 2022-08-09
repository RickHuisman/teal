#include "syntax/lexer.h"
#include "syntax/parser.h"
#include "vm/vm.h"

void CompileAndRun(const std::string& source) {
  VM vm;
  vm.interpret(source);
}

int main() {
  auto source = R"(print !true;)";

  CompileAndRun(source);

  return 0;
}
