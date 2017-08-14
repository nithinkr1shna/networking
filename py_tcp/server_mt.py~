import socket

IP = "127.0.0.1"
PORT = 3490
BUFFER_SIZE = 100

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((IP, PORT))
s.listen(1)

conn, addr = s.accept()
print 'connection address', addr
while 1:
  data = conn.recv(BUFFER_SIZE)
  if not data: break
  print "received data",data
  conn.send(data)
conn.close()
