import socket

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("127.0.0.1", 8888))

    while True:
        msg, sender = sock.recvfrom(4096)
        msg = msg.decode()
        print(f"Received {msg} from {sender}")

        resp = msg.upper()
        sock.sendto(resp.encode(), sender)

if __name__ == "__main__":
    main()
