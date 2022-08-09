#include "syntax/lexer.h"
#include "syntax/parser.h"
#include "vm/vm.h"

void CompileAndRun(const std::string& source) {
  VM vm;
  vm.interpret(source);
}

int main() {
  auto source = R"(
let x = 10;
x = 4;
print x;
)";

  CompileAndRun(source);

  return 0;
}
