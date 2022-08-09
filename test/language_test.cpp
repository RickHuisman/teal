#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vm/vm.h>
#include <regex>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

std::vector<std::string> execute(const std::string &source) {
  VM vm;

  std::stringstream buffer;
  vm.interpret_with_output(source, &buffer);

  std::vector<std::string> output;
  std::string line;
  while (buffer >> line) {
    output.push_back(line);
  }

  return output;
}

std::string get_contents(const std::string &path) {
  std::ifstream ifs(path);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  return content;
}

std::vector<std::string> parse_expects(std::string source) {
  std::vector<std::string> results;

  std::regex regex("// expect: ?(.*)");
  std::smatch m;

  while (regex_search(source, m, regex)) {
    // Strip "// expect" from match.
    auto result =
        std::regex_replace(m[0].str(), std::regex("// expect: "), "$1");
    results.push_back(result);

    source = m.suffix();
  }

  return results;
}

void run_test_file(const std::string &source) {
  auto expects = parse_expects(source);

  auto output = execute(source);

  EXPECT_EQ(expects, output);
}

std::vector<std::string> collect_test_file_paths() {
  std::vector<std::string> paths;

  auto path = "/Users/rickhuisman/Documents/cpp/teal/test/tests"; // TODO: Fix path.
  for (const auto &entry : recursive_directory_iterator(path)) {
    if (entry.is_regular_file()) {
      paths.push_back(entry.path());
    }
  }

  return paths;
}

TEST(language_tests, run_test_suite) {
  auto test_paths = collect_test_file_paths();

  for (const auto &path : test_paths) {
    auto source = get_contents(path);
    run_test_file(source);
  }
}
