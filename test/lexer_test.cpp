#include "gtest/gtest.h"
#include "syntax/lexer.h"

TEST(lexer, lex_numbers) {
  auto source = "2 10 3.33";

  std::vector<Token> expect{
      Token(TokenType::Number, "2"),
      Token(TokenType::Number, "10"),
      Token(TokenType::Number, "3.33"),
      Token(TokenType::Eof, ""),
  };

  Lexer lexer(source);
  auto tokens = lexer.Lex();

  ASSERT_EQ(expect, tokens);
}

TEST(lexer, lex_keywords) {
  auto source = R"(let
        true
        false
        print
        do
        end
        return)";

  std::vector<Token> expect{
      Token(TokenType::Let, "let"),
      Token(TokenType::Line, ""),
      Token(TokenType::True, "true"),
      Token(TokenType::Line, ""),
      Token(TokenType::False, "false"),
      Token(TokenType::Line, ""),
      Token(TokenType::Print, "print"),
      Token(TokenType::Line, ""),
      Token(TokenType::Do, "do"),
      Token(TokenType::Line, ""),
      Token(TokenType::End, "end"),
      Token(TokenType::Line, ""),
      Token(TokenType::Return, "return"),
      Token(TokenType::Eof, ""),
  };

  Lexer lexer(source);
  auto tokens = lexer.Lex();

  ASSERT_EQ(expect, tokens);
}
