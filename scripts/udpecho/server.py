#
# Basic UDP server.
#
# @author Michel Megens
# @email  dev@bietje.net
#

import socket
import sys


class UdpServer(object):
    def __init__(self, bindaddr, port):
        self.bindaddr = bindaddr
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def start(self):
        self.socket.bind((self.bindaddr, self.port))

        while True:
            data, addr = self.socket.recvfrom(1024)
            print("Received data: %s" % data)

            if data:
                self.socket.sendto(data, addr)


def print_help(script):
    print("Usage: %s <bind-address> <port>" % script)


def main():
    if len(sys.argv) < 3:
        print_help(sys.argv[0])

    try:
        port = int(sys.argv[2])
        host = sys.argv[1]
        server = UdpServer(host, port)
        server.start()
    except ValueError:
        print_help(sys.argv[0])


if __name__ == "__main__":
    main()
