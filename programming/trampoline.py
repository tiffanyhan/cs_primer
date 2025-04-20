def trampoline(f):
    # TODO have a kind of accumulator
    # keep calling the given fn
    # until the return value (what we're setting to our accumulator)
    # is no longer a funciton
    # then return that value
    while callable(f):
        f = f()

    return f


def fib(n, acc=1):
    if n in (1,2):
        return acc
    return n * factorial(n-1)

if __name__ == "__main__":
    print(factorial(1000))
