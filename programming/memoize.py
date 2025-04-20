import urllib.request    

def fetch(url):
    with urllib.request.urlopen(url) as response:
        content = response.read().decode('utf-8')
        return content


def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

def memoize(f):
    cache = dict()
    def memoized(*args):
        try:
            resp = cache[args]
            print("got cached result for args:", args)
        except KeyError:
            resp = f(*args)
            cache[args] = resp
            print("fetched or computed result and set the output in the cache for args:", args)
        return resp
    return memoized

if __name__ == '__main__':
    memoized_fetch = memoize(fetch)
    print(memoized_fetch('http://google.com')[:80])
    print(memoized_fetch('http://google.com')[:80])

    fib = memoize(fib)
    print(fib(10))
