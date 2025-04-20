#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MASK 0x07fffffe

/*

0x1f
0001 1111

'a' = 65 or 0x41
0100 0001
0001 1111 &
        1

0x1f
in binary, this mask is 0b 0001 1111
it's saying only look at first 5 bits
in binary, 'a' and 'A' will have the same first 5 bits
if you only look at lower order 5 bits, 'a' and 'A' will both map to 1

01000001 = 65 or 'A' (64+1)
01100001 = 97 or 'a' (64+32+1)

what happens to non-alpha chars with codes 91-96?
in my C solution, we do a simple equality comparison with `bitset == TARGET_MASK`
in this solution, we do `bitset & TARGET_MASK == TARGET_MASK`
in order to account for the non-alpha chars in positions 91-96
*/
bool ispangram(char *s) {
  uint32_t bs = 0;
  char c;
  while ((c = *s++) != '\0') {
    if (c < '@')
      continue; // ignore first 64 chars in ascii table
    bs |= 1 << (c & 0x1f); // mask is 0b 0001 1111, only look at first 5 bits
  }
  return (bs & MASK) == MASK;
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
