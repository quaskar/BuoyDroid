import socket
from struct import *
import time
import foxglove
from foxglove import Channel
from foxglove.schemas import Log, LogLevel, Timestamp

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 5001))

print("Warte auf UDP...")

SignalDecodeStr = ""
SignalNames = []

foxglove.start_server()
BouyBotChannel = Channel("/bouybot", message_encoding="json")

while True:
    
    # Read from socket
    data, addr = sock.recvfrom(1024)

    # skip if messag eis too small
    if len(data) < 4:
        continue

    # decode message id and magic number
    (MagicNum, MessageId) = unpack('<HH', data[0:4])
    Data = data[4:]

    if MagicNum == 1375:

        # Logging 
        if MessageId == 0:
            foxglove.log("/log", Log(timestamp=Timestamp.now(), level=LogLevel.Info, message=Data.decode(),))

        # Signal Announcement
        elif MessageId == 1:
            print (Data.decode())

            SignalDecodeStr = Data.decode().split('|')[1]
            SignalDecodeStr = SignalDecodeStr[0] + SignalDecodeStr[3:]
            SignalNames = Data.decode().split('|')[0].split(',')[3:]

            print (SignalNames)
            print (SignalDecodeStr)

        # Signal Data 
        elif MessageId == 2:

            if len(SignalDecodeStr) <= 0:
                   continue

            Values = unpack(SignalDecodeStr, Data)
            SignalData = {}
            for i in range(0,len(SignalNames)):
                SignalData[SignalNames[i]] = Values[i]
            BouyBotChannel.log(SignalData)

        else:
            pass
