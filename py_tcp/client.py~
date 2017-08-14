
import socket

IP ="127.0.0.1"
PORT = 3490
BUFFER_SIZE =100

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((IP, PORT))
while 1:
  data = raw_input()
  s.send(data.encode('utf-8'))
  data = s.recv(BUFFER_SIZE)
  print "recieved", data
s.close()
