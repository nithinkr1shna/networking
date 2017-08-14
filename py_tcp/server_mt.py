import socket
from threading import * 

IP = "127.0.0.1"
PORT = 3490
BUFFER_SIZE = 100

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((IP, PORT))

class Server(Thread):
  def __init__(self, socket, address):
    self.socket = socket
    self.address = address
    self.start()

  def run(self):
    while 1:
      data = self.socket.recv(BUFFER_SIZE).decode('utf-8')
      print "From client:",data
      self.socket.send(data.encode('utf-8'))

s.listen(10)


while 1:
  sock, addr = s.accept()
  client(sock, addr)
conn.close()
