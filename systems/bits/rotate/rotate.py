"""
42
0100 0010
64   2

4d
0100 1101
64   13

The first 2 bytes is "BM"
The next 4 bytes is the size of the file
The next 2 bytes are reserved
The next 2 bytes are also reserved
The next 4 bytes are the starting address where the image data (pixel array can be found)
  - Offset is 10 in decimal or 0a in hex
The next 4 bytes are the size of the header
The next 4 bytes are the bitmap width in pixels (unsigned 16-bit)
  - offset is 18 in decimal or 12 in hex
The next 4 bytes are the bitmap neight in pixels (unsigned 16-bit)
  - offset is 22 in decimal or 16 in hex

The starting address is 0x8a
  - 138 in decimal
  - 8a in hex

8a
1000 1010
128  10
=138

x x x x

The width and height is a4 01 in little endian or 420x420 pixels
a4
1010 0100
=164

01
=1

1010 0100  0000 0001
0000 0001  1010 0100
256        164
= 420

"""
def _read_bytes_as_int_little_endian(bs: bytes) -> int:
    num = 0
    for byte in reversed(bs):
        num <<= 8
        num += byte

    return num

"""
3x3 pixels:

      0x00 0x00 0x00 | 0x00 0x00 0x00 | 0x00 0x00 0x00
i:    0    1    2    | 3    4    5    | 6    7    8
col:  0    1    2    | 0    1    2    | 0    1    2
row:  0              | 1              | 2

i // 3 = row
i % 3  = col

3x2 pixels:
wxh

      0x00 0x00 0x00 | 0x00 0x00 0x00 
i:    0    1    2    | 3    4    5    
col:  0    1    2    | 0    1    2    
row:  0              | 1              

i // 3 = row
i % 3  = col
"""

def rotate_image(bs: bytes):
  offset = _read_bytes_as_int_little_endian(bs[10:14])
  width_pixels = _read_bytes_as_int_little_endian(bs[18:22])
  height_pixels = _read_bytes_as_int_little_endian(bs[22:26])

  print("image starts at offset:", offset)
  print("width x height of image in pixels:", width_pixels, height_pixels)
  print("first pixel bytes:", hex(bs[138]), hex(bs[139]))

  arr = [[] for _ in range(height_pixels)]
  width_bytes = width_pixels * 3

  for i, byte in enumerate(bs[offset:]):
    row = i // width_bytes
    arr[row].append(byte)

  # for y in range(offset, (height * 3) + 1):
  #   for x in range(offset, (width * 3) + 1):
  #     pass

  # rows: 420
  # cols: 1260
  print("rows:", len(arr))
  print("cols:", len(arr[0]))

  # transpose
  # for y in range(len(arr)):
  #   for x_bytes in range(0, len(arr[0]), 3):
  #     x_pixels = x_bytes // 3
  #     arr[x_pixels][y:y+3] = arr[y][x_bytes:x_bytes+3]

  # reverse
  for y in range(len(arr)):
    arr[y].reverse()

  path = "my-rotated.bmp"
  with open(path, 'wb') as f:
    f.write(bs[:offset])

    for y in range(len(arr)):
      for x in range(len(arr[0])):
        byte_as_int = arr[y][x]
        f.write(byte_as_int.to_bytes())

if __name__ == "__main__":
    with open("teapot.bmp", "rb") as f:
        data = f.read()
        rotate_image(data)
