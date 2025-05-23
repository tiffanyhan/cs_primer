#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


bool ispangram(char *s) {
  int bitset = 0;

  for (size_t i = 0; i < strlen(s); i++) {
    char chr = s[i];
    if (isalpha(chr)) {
      chr = tolower(chr);
      int mask = 1;
      mask <<= chr - 'a';
      bitset |= mask;
    }
  }

  return bitset == 0x03ffffff;
}

int main() {
  size_t len;
  ssize_t read;
  char *line = NULL;
  while ((read = getline(&line, &len, stdin)) != -1) {
    if (ispangram(line))
      printf("%s", line);
  }

  if (ferror(stdin))
    fprintf(stderr, "Error reading from stdin");

  free(line);
  fprintf(stderr, "ok\n");
}
