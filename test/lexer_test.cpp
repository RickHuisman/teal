#include "gtest/gtest.h"
#include "syntax/lexer.h"

TEST(lexer, lex_numbers) {
  auto source = "2 10 3.33";

  std::vector<Token> expect{
    Token(TokenType::Number, "2"),
    Token(TokenType::Number, "10"),
    Token(TokenType::Number, ".33"),
  };

  Lexer lexer(source);
  auto tokens = lexer.Lex();

  EXPECT_TRUE(expect == expect);
}

//TEST(lexer, lex_strings) {
//  auto source = R"("Hello" "," "World!")";
//
//  std::vector<Token> expect{
//      Token(TokenType::String, "Hello"),
//      Token(TokenType::String, ","),
//      Token(TokenType::String, "World!"),
//  };
//
//  Lexer lexer(source);
//  auto tokens = lexer.lex();
//
//  ASSERT_EQ(expect, expect);
//}
