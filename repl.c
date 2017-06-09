#include "config.h"

#include <ctype.h>  // isspace, etc
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

#include "repl.h"
#include "dictionary.h"
#include "tty.h"
#include "execute.h"

jmp_buf catch;

void die(const char * msg) {
  println(msg);
  longjmp(catch, 1);
}

int isdquote(int c) {
  return c == '"';
}

int issquote(int c) {
  return c == '\'';
}

char * readstr(char * buf, int (*ateof)(int)) {
  char * end = buf;
  while (!ateof(*end) && *end != '\0') ++end;
  if (!*end) die("Unterminated string literal");

  char * str = malloc(end-buf+1);
  strncpy(str, buf, end-buf);
  str[end-buf] = '\0';
  push((intptr_t)str);
  return end+1;
}

// Run the word at the head of the buffer.
// On error, report the error and then longjmp.
// Return pointer to first character past word.
char * run_word(char * buf) {
  // Comments are dropped at readline() time to save space in the input buffer,
  // so we don't need to check for them here.

  // Is it a numeric constant?
  if (isdigit(buf[0])
      || (isdigit(buf[1]) && (buf[0] == '-' || buf[0] == '+'))) {
    char * end;
    push(strtol(buf, &end, 0));
    return end;
  }

  // Is it a string literal?
  if (buf[0] == '"') {
    return readstr(buf+1, isdquote);
  }
  if (buf[0] == '\'') {
    return readstr(buf+1, issquote);
  }
  if (buf[0] == ':') {
    return readstr(buf+1, isspace);
  }

  // Is it addressof?
  char addressof = false;
  if (buf[0] == '&') {
    buf++;
    addressof = true;
  }

  // Otherwise it's a word.
  char * end = buf;
  while (!isspace(*end) && *end != '\0') ++end;
  // At this point it points to either the end of the buffer, which is already \0,
  // or to the whitespace separating this word from the next, which we can safely
  // discard
  if (*end) *(end++) = '\0';

  // Now buf points to the start of the word and is null terminated
  Word* word = find_word(buf);
  if (!word) {
    print("Word not found in dictionary: ");
    die(buf);
  }
  if (addressof) {
    push((intptr_t)word);
  } else {
    execute_word(word);
  }
  return end;
}

// Execute the code inside buf.
// MODIFIES THE CONTENTS OF BUF. Handle with care.
void run_buffer(char * buf) {
  if(setjmp(catch)) {
    // An error was thrown inside the execution loop.
    return;
  }
  // process each word in the buffer
  while (1) {
    // Skip leading whitespace
    while (isspace(*buf)) ++buf;
    if (*buf == '\0') return;
    buf = run_word(buf);
  }
}

extern Word* compiling;
void repl() {
  char buf[INPUT_BUFSIZE+1];
  while (1) {
    if (compiling) print("compile ");
    print("[");
    printint(STACKP);
    print("] ");
    readline(buf, INPUT_BUFSIZE);
    run_buffer(buf);
  }
}
