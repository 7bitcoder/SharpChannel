import sys

echo = ''
if( len(sys.argv) >= 2):
    echo = sys.argv[1]

print(echo, end = '')