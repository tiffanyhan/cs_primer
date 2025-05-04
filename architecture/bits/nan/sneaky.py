import math
import struct

from typing import Callable

"""
32 bits
11111111 10000000 00000000 00000001
se       n

64 bits
11111111 11110000 00000000 00000000 00000000 00000000 00000000 00000001
se          n
"""

"""
if the payload is greater than limit, then the payload
will be truncated to fit the limit.

if the payload is shorter than limit, then it will
right pad the payload with 0x00.
"""
def makeConceal(prefix: bytes, format: str, limit: int) -> Callable[[str], float]:
    def f(s: str) -> float:
        s = s[:limit]
        output = s.encode("ascii")
        padding = b'\x00' * max(0, limit - len(s))
        return struct.unpack(format, prefix + output + padding)[0]
    return f

def makeExtract(format: str) -> Callable[[float], str]:
    def f(n: float) -> str:
        bs = struct.pack(format, n)
        concealed = bs[2:].replace(b'\x00', b'')
        return concealed.decode('ascii')
    return f

conceal32 = makeConceal(b'\xff\x80', ">f", 2)
extract32 = makeExtract(">f")

conceal64 = makeConceal(b'\xff\xf0', ">d", 6)
extract64 = makeExtract(">d")

def main():
    # 32-bit version
    x = conceal32("hiiiiii")
    assert type(x) == float
    assert math.isnan(x)

    s = extract32(x)
    assert s == "hi"

    # 64-bit version
    y = conceal64("helloooo")
    assert type(y) == float
    assert math.isnan(y)

    s2 = extract64(y)
    assert s2 == "helloo"

    # 64-bit version: shorter str
    z = conceal64("hey")
    assert type(z) == float
    assert math.isnan(z)

    s3 = extract64(z)
    assert s3 == "hey"

if __name__ == "__main__":
    main()
