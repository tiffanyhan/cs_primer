def _hex_to_int(hex: str) -> int:
    return int(hex, 16)

def hex_to_rgb(hex: str) -> str:
    red = _hex_to_int(hex[1:3])
    green = _hex_to_int(hex[3:5])
    blue = _hex_to_int(hex[5:7])
    return f"rgb({red} {green} {blue})"

if __name__ == "__main__":
    assert hex_to_rgb('#00ff00') == 'rgb(0 255 0)'
    assert hex_to_rgb('#fe030a') == 'rgb(254 3 10)'
    assert hex_to_rgb('#0f0def') == 'rgb(15 13 239)'
