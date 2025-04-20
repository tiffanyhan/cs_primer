def _hex_to_int(hex: str) -> int:
    return int(hex, 16)

def hex_to_rgb(hex: str) -> str:
    red = _hex_to_int(hex[1:3])
    green = _hex_to_int(hex[3:5])
    blue = _hex_to_int(hex[5:7])
    return f"rgb({red} {green} {blue})"

if __name__ == "__main__":
    # https://yizzle.com/whatthehex/?n=2
    assert hex_to_rgb('#00ff00') == 'rgb(0 255 0)'
    assert hex_to_rgb('#fe030a') == 'rgb(254 3 10)'
    assert hex_to_rgb('#0f0def') == 'rgb(15 13 239)'

    """
    # fe 03 0a
    1111 1110
    240  14

    0000 0011
         3

    0000 1010
         10

    # 0f 0d ef

    0000 1111
         15

    0000 1101
         13

    1110 1111
    224  15

    # 77 5A AB

    77
    0111 0111
    112  7
    =119

    5A
    0101 1010
    80   10
    =90

    AB
    1010 1011
    160  11
    =171

    #b04d8d

    b0
    1011 0000
    =176

    4d
    0100 1101
    64   13
    =77

    8d
    1000 1101
    128  13
    =141

    """
