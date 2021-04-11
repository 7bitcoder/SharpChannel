import socket
import sys
from enum import Enum

class TestScenario(Enum):
    Simple = 1

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 65432        # The port used by the server

if( len(sys.argv) >= 2):
    testScenario = TestScenario(int(sys.argv[1]))
if( len(sys.argv) >= 3):
    PORT = int(sys.argv[2])
if( len(sys.argv) >= 4):
    exit(-1)

if testScenario == TestScenario.Simple:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b'Hello, world')
        data = s.recv(1024)
    