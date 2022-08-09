#include "morph.h"

void strip_leading_lines(std::vector<Token> *tokens) {
  for (;;) {
    auto first = tokens->front();
    if (first.type == TokenType::Line) {
      tokens->erase(tokens->begin());
    } else {
      return;
    }
  }
}

std::vector<Token> morph_duplicate_lines(std::vector<Token> tokens) {
  std::vector<Token> morphed;

  while (!tokens.empty()) {
    auto token = tokens.back();
    tokens.pop_back();

    if (token.type == TokenType::Line) {
      auto last_token_type = morphed.back().type;
      if (last_token_type != TokenType::Line) {
        morphed.push_back(token);
      }
    } else {
      morphed.push_back(token);
    }
  }

  std::reverse(morphed.begin(), morphed.end());
  return morphed;
}

void morph_single_line_expr(std::vector<Token> *tokens) {
  // Pop `Eof` token.
  tokens->pop_back();

  if (tokens->back().type == TokenType::Line) return;

  // Add `Line` token.
  Token line_token(TokenType::Line, "");
  tokens->push_back(line_token);

  // Add `Eof` token.
  Token eof_token(TokenType::Eof, "");
  tokens->push_back(eof_token);
}

std::vector<Token> morph(std::vector<Token> tokens) {
  strip_leading_lines(&tokens);
  auto morphed = morph_duplicate_lines(tokens);
  morph_single_line_expr(&morphed);
  return morphed;
}