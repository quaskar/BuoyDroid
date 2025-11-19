import socket
from struct import *
import time
import foxglove
from foxglove import Channel
from foxglove.schemas import Log, LogLevel, Timestamp

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 5001))

print("Warte auf UDP...")

data, addr = sock.recvfrom(1024)
print(f"{addr}: {data.decode()}")
DecodeStr = data.decode().split('|')[1]
Names = data.decode().split('|')[0].split(',')[1:]

print (DecodeStr)
print(Names)

server = foxglove.start_server()


# create channels 
BouyBotChannel = Channel("/bouybot", message_encoding="json")

i = 0
while True:
    data, addr = sock.recvfrom(1024)
    print(len(data))
    print(unpack(DecodeStr, data))

    #foxglove.log(
    #    "/hello",
    #    Log(
    #        timestamp=Timestamp.now(),
    #        level=LogLevel.Info,
    #        message="Hallo",
    #    )
    #)

    Values = unpack(DecodeStr, data)
    data = {}
    for i in range(0,len(Names)):
        data[Names[i]] = Values[i]
    BouyBotChannel.log(data)
