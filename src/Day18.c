#include "Day18.h"

#include <stdio.h>

#include "parsing.h"

/// Day 18: Operation Order
///
/// Parse math expressions with different precedence rules than normal.

#define MAX_TOKENS 100  ///< Maximum number of tokens in a line

/// The different tokens our parser might encounter
typedef enum {
  TOK_NULL,         ///< A placeholder token to represent {0}
  TOK_NUM,          ///< A digit 0-9 (no double-digits in input)
  TOK_OPEN_PAREN,   ///< (
  TOK_CLOSE_PAREN,  ///< )
  TOK_PLUS,         ///< +
  TOK_STAR,         ///< *
  TOK_END,          ///< End of line sentinel
} TokenType;

const char* token_type[] = {"Null", "Num", "Open Paren", "Close Paren", "Plus", "Star", "End"};

/// A Token is essentially just a TokenType, but NUM tokens can store their value
typedef struct {
  TokenType type;
  long val;
} Token;

/// An iteratable of Tokens, that can be consumed and used up
/// but is also really just a list of Tokens with a memory
typedef struct {
  Token* tokens;
  Token* current;
} TokenIter;

/// Parse a line of input into a TokenIter
TokenIter parse_line(FILE* fp) {
  Token* tokens = (Token*)malloc(sizeof(Token) * MAX_TOKENS);
  int i = 0;
  char c;
  while ((c = getc(fp)) != EOF) {
    switch (c) {
      case '0' ... '9':
        tokens[i].type = TOK_NUM;
        tokens[i].val = c - '0';
        break;
      case '(': tokens[i].type = TOK_OPEN_PAREN; break;
      case ')': tokens[i].type = TOK_CLOSE_PAREN; break;
      case '+': tokens[i].type = TOK_PLUS; break;
      case '*': tokens[i].type = TOK_STAR; break;
      case '\n': 
        tokens[i].type = TOK_END; 
        return (TokenIter){
          .tokens = tokens,
          .current = NULL,
        };
      case ' ': continue;
      default: printf("Bad char %c.\n", c); exit(EXIT_FAILURE);
    }
    i++;
  }

  // May encounter end of file without newline
  tokens[i].type = TOK_END;
  return (TokenIter){
    .tokens = tokens,
    .current = NULL,
  };
}

/// Parse input file so that each line is an iter of Tokens
TokenIter* parse(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open input file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_lines(fp);
  TokenIter* lines = (TokenIter*)malloc(sizeof(TokenIter) * *count);
  
  for (int i = 0; i < *count; i++) {
    lines[i] = parse_line(fp);
  }

  fclose(fp);
  return lines;
}

/// Iterate to next token (or prime the pump on a fresh iterable)
Token* token_next(TokenIter* iter) {
  if (iter->current == NULL) {
    iter->current = iter->tokens;
    return iter->current;
  }
  if (iter->current->type == TOK_END) return iter->current;
  return ++iter->current;
}

/// Short helper function to actually interpret operators
long do_op(TokenType op, long a, long b) {
  switch (op) {
    case TOK_PLUS: return a + b;
    case TOK_STAR: return a * b;
    default: printf("Unsupported op type: %d\n", op); exit(EXIT_FAILURE);
  }
}

/// Evaluate a line of input with only L-to-R and () precedence
long evaluate(TokenIter* tokens) {
  Token* t;
  long val_stack[MAX_TOKENS] = {0};
  int sp = 0;
  TokenType op_stack[MAX_TOKENS] = {0};
  int op_sp = 0;
  for (int i = 0; (t = token_next(tokens))->type != TOK_END; i++) {
    switch (t->type) {
      case TOK_NUM: 
        if (op_sp > 0) {
          val_stack[sp - 1] = do_op(op_stack[--op_sp], t->val, val_stack[sp - 1]);
        } else {
          val_stack[sp++] = t->val; 
        }
        break;
      case TOK_PLUS: op_stack[op_sp++] = TOK_PLUS; break;
      case TOK_STAR: op_stack[op_sp++] = TOK_STAR; break;
      case TOK_OPEN_PAREN: {
          long val = evaluate(tokens);
          if (op_sp > 0) {
            val_stack[sp - 1] = do_op(op_stack[--op_sp], val_stack[sp - 1], val);
          } else {
            val_stack[sp++] = val;
          }
        };
        break;
      case TOK_CLOSE_PAREN:
        if (op_sp > 0) {
          printf("End paren with unresolved operators.\n");
          exit(EXIT_FAILURE);
        }
        if (sp > 1) {
          printf("Unresolved integers on the stack.\n");
          exit(EXIT_FAILURE);
        }
        return val_stack[0];
      default: break;
    }
  }
  if (op_sp > 0) {
    printf("End paren with unresolved operators.\n");
    exit(EXIT_FAILURE);
  }
  if (sp > 1) {
    printf("Unresolved integers on the stack.\n");
    exit(EXIT_FAILURE);
  }
  return val_stack[0];
}

/// Given only paren and L-to-R precedence, add up the result of
/// evaluating each line of input
long part1(const char* filename) {
  int count;
  TokenIter* lines = parse(filename, &count);

  long total = 0;
  for (int i = 0; i < count; i++) {
    total += evaluate(&lines[i]);
    free(lines[i].tokens);
  }
  
  free(lines);
  return total;
}

/// Helper macro for unraveling the stack of operators.
/// Each operator pops two values off the value stack, operates on them
/// and disappears, pushing the result back on the value stack.
#define UNRAVEL_OPS \
  while (op_sp != 0) { \
    long a = val_stack[--sp]; \
    long b = val_stack[--sp]; \
    val_stack[sp++] = do_op(op_stack[--op_sp], a, b); \
  }

/// Evaluate a line of input with addition taking precedence over 
/// multiplication, and () being highest precedence
long evaluate_plus_prec(TokenIter* tokens) {
  Token* t;
  long val_stack[MAX_TOKENS] = {0};
  int sp = 0;
  TokenType op_stack[MAX_TOKENS] = {0};
  int op_sp = 0;
  for (int i = 0; (t = token_next(tokens))->type != TOK_END; i++) {
    switch (t->type) {
      case TOK_NUM: val_stack[sp++] = t->val; break;
      case TOK_STAR: 
        UNRAVEL_OPS;
        op_stack[op_sp++] = TOK_STAR; 
        break;
      case TOK_PLUS: op_stack[op_sp++] = TOK_PLUS; break;
      case TOK_OPEN_PAREN: val_stack[sp++] = evaluate_plus_prec(tokens); break;
      case TOK_CLOSE_PAREN:
        UNRAVEL_OPS;
        if (sp > 1) {
          printf("Unresolved integers on the stack.\n");
          exit(EXIT_FAILURE);
        }
        return val_stack[0];
      default: break;
    }
  }
  UNRAVEL_OPS;
  if (sp > 1) {
    printf("Unresolved integers on the stack.\n");
    exit(EXIT_FAILURE);
  }
  return val_stack[0];
}

#undef UNRAVEL_OPS

/// Add up the result of each line of input assuming precedence goes
/// () before + before *.
long part2(const char* filename) {
  int count;
  TokenIter* lines = parse(filename, &count);

  long total = 0;
  for (int i = 0; i < count; i++) {
    total += evaluate_plus_prec(&lines[i]);
    free(lines[i].tokens);
  }
  
  free(lines);
  return total;
}

/// Run both parts
int day18() {
  printf("====== Day 18 ======\n");
  printf("Part 1: %ld\n", part1("data/day18.txt"));
  printf("Part 2: %ld\n", part2("data/day18.txt"));
  return EXIT_SUCCESS;
}
