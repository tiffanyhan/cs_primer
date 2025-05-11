"""
10xx xxxx
1100 0000 & 0xc0
1000 0000 = 0x80

byte1       byte2       byte3       byte4
11110uvv	10vvwwww	10xxxxyy	10yyzzzz

                                    max_bytes

check the byte that comes immediately after the
max limit. if it's a leading byte, we're safe to
cut it off here and return everything up to this point (non-inclusive).
if it's not a leading byte, we need to decrement the cutoff point
until we're at a safe index.
"""
def truncate(max_bytes: int, bs: bytes) -> bytes:
    if max_bytes >= len(bs):
        return bs

    is_cont_byte = lambda b: (b & 0xc0) == 0x80

    trunc_index = max_bytes
    while trunc_index > 0 and is_cont_byte(bs[trunc_index]):
        # decrement until the index points to a leading byte
        trunc_index -= 1

    return bs[:trunc_index]

"""
0xc3 0xa9
1100 0011 1010 1001
mmmp ppxx xxxx xxxx
          169
          128
          256

"""
if __name__ == "__main__":
    truncated_lines = []

    with open('cases', 'rb') as file:
        for line in file:
            truncated_lines.append(truncate(line[0], line[1:-1]))

    with open('actual', 'wb') as file:
        for line in truncated_lines:
            file.write(line + b'\n')
