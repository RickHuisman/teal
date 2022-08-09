#include "gtest/gtest.h"
#include "syntax/lexer.h"
#include "syntax/morph.h"

TEST(morph_tokens, morph_leading_lines) {
  auto source = R"(


  10)";

  std::vector<Token> expect{
      Token(TokenType::Number, "10"),
      Token(TokenType::Line, ""),
      Token(TokenType::Eof, "")
  };

  Lexer lexer(source);
  auto tokens = lexer.Lex();
  auto morphed = morph(tokens);

  ASSERT_EQ(expect, morphed);
}

TEST(morph_tokens, morph_duplicate_lines) {
  auto source = R"(
  10


  )";

  std::vector<Token> expect{
      Token(TokenType::Number, "10"),
      Token(TokenType::Line, "")
  };

  Lexer lexer(source);
  auto tokens = lexer.Lex();
  auto morphed = morph(tokens);

  ASSERT_EQ(expect, morphed);
}

TEST(morph_tokens, morph_single_line_expr) {
  auto source = R"(10)";

  std::vector<Token> expect{
      Token(TokenType::Number, "10"),
      Token(TokenType::Line, ""),
      Token(TokenType::Eof, "")
  };

  Lexer lexer(source);
  auto tokens = lexer.Lex();
  auto morphed = morph(tokens);

  ASSERT_EQ(expect, morphed);
}