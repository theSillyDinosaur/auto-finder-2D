from time import sleep
import serial
# these codes are for bluetooth
# hint: please check the function "sleep". how does it work?

class bluetooth:
    def __init__(self):
        self.ser = serial.Serial()

    def do_connect(self,port):
        self.ser.close()
        print("Connecting...")
        try:
            self.ser = serial.Serial(port,9600,timeout=2)
            print("connect success")
            print("")
        except serial.serialutil.SerialException:
            print("fail to connect")
            print("")
            return False
        return True

    def connect_prog(self):
        print("")
        print("Arduino Bluetooth Connect Program.")
        print("")
        port = input("PC bluetooth port name: ")
        while(not self.do_connect(port)):
            if(port == "quit"):
                self.ser.disconnect()
                quit()
            port = input("PC bluetooth port name: ")

    def disconnect(self):
        self.ser.close()

    def SerialWrite(self,output):
        # send = 's'.encode("utf-8")
        send = output.encode("utf-8")
        self.ser.write(send)

    def SerialReadString(self):
        # TODO: Get the information from Bluetooth. Notice that the return type should be transformed into hex.
        waiting = self.ser.in_waiting
        if waiting >= 0:
            rv = self.ser.read(1).decode("utf-8") 
            return rv
        return ""
    # confront a node: 0
    # scan a RFID: 1 + UID code

    def SerialReadByte(self):
        sleep(0.05)
        waiting = self.ser.inWaiting()
        rv = self.ser.read(waiting)
        if(rv):
            UID = hex(int.from_bytes(rv, byteorder='big', signed=False))
            self.ser.flushInput()
            return UID
        else:
            return 0
    
    def EndlessReadUID(self, point):
        if self.ser.inWaiting():
            UID = str("")
            for i in range(9):
                while self.ser.inWaiting() == 0:
                    sleep(0.01)
                if i == 8:
                    self.ser.read(1).decode("utf-8")
                    break
                UID += self.ser.read(1).decode("utf-8")
            print(UID)
            point.add_UID(UID)
            print(point.getCurrentScore())
            return UID


if __name__ == "__main__":
    bt = bluetooth()
    bt.connect_prog()
    bt.SerialWrite("a")