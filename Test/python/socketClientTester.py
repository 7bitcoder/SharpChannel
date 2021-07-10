#!/usr/bin/env python3
import sys
from enum import Enum
import socket
import time

class TestScenario(Enum):
    SimplePong = 1
    MessageReceive = 2

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)
testScenario = TestScenario.SimplePong



if( len(sys.argv) >= 2):
    testScenario = TestScenario(int(sys.argv[1]))
if( len(sys.argv) >= 3):
    PORT = int(sys.argv[2])
if( len(sys.argv) >= 4):
    exit(-1)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        data = conn.recv(1024)
        conn.sendall(data)
        time.sleep(0.1)