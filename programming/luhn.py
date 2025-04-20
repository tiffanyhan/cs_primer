def isOdd(n: int) -> bool:
    return n % 2 == 1

def mapDigit(i: int, digit: int) -> int:
    if isOdd(i):
        doubled = digit * 2
        if doubled > 9:
            doubled -= 9
        return doubled
    return digit

def verify(digits: str) -> bool:
    reversed = digits[::-1]
    acc = 0
    for i in range(len(reversed)):
        acc += mapDigit(i, int(reversed[i]))
    return acc % 10 == 0

print(verify("79927398713"))
