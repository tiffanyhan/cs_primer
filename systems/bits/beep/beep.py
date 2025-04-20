if __name__ == "__main__":
    import time

    while True:
        for i in range(65):
            print(chr(i))

        num = input("Enter a number: ")
        for _ in range(int(num)):
            print(chr(7))
            time.sleep(1)
