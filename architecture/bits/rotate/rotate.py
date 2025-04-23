def _read_bytes_as_int_little_endian(bs: bytes) -> int:
    num = 0
    for byte in reversed(bs):
        num <<= 8
        num += byte

    return num


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


  # rows: 420
  # cols: 1260
  print("rows:", len(arr))
  print("cols:", len(arr[0]))

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
