import struct
import sys

# def decode(bytes):
#     nums = struct.unpack('>Q', bytes)
#     return nums[0]

"""
0000 0000

assert encode(150) == b'\x96\x01'

96 01
1001 0110 0000 0001
 001 0110  000 0001
 000 0001  001 0110
          1001 0110


"""
def encode(n: int) -> bytes:
    out = []

    while n > 0:
        lower_7 = n % 128
        n >>= 7
        msb = 128 if n > 0 else 0
        out.append(lower_7 + msb)

    return bytes(out)

"""
\x96      \x01
1001 0110 0000 0001
 001 0110  000 0001

\x01      \x16
 000 0001  001 0110
          1001 0110

1

"""
def decode(bs: bytes) -> int:
    n = 0

    for b in reversed(bs):
        lower_7 = b % 128
        n <<= 7
        n += lower_7
    return n


if __name__ == "__main__":
    args = sys.argv[1:]

    cases = (
        ('1.uint64', b'\x01'),
        ('150.uint64', b'\x96\x01'),
        ('maxint.uint64', b'\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01'),
    )
    for fname, expectation in cases:
        with open(fname, 'rb') as f:
            n = struct.unpack('>Q', f.read())[0]
            assert encode(n) == expectation
            assert decode(encode(n)) == n
