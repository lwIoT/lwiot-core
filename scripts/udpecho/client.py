#
# Basic UDP server.
#
# @author Michel Megens
# @email  dev@bietje.net
#

import socket
import sys


def print_help(script):
    print("Usage: %s <host> <port>" % script)


def main():
    if len(sys.argv) < 3:
        print_help(sys.argv[0])

    host = sys.argv[1]
    port = int(sys.argv[2])
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM,)
    sock.sendto("Hello, World!", (host, port))
    print("Sent datagram to server!")
    data, addr = sock.recvfrom(1024)
    print("Received from server: {}".format(data))


if __name__ == '__main__':
    main()
