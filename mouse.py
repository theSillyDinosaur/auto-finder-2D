import time
import pyautogui as pag
import BT

class Mouse:
    def __init__(self, BT):
        self.screenWidth, self.screenHeight = pag.size() #獲取螢幕的尺寸
        self.scale = 1
        pag.FAILSAFE = False
        pag.moveTo(self.screenWidth/2,750)
        print("Mouse on position.")
        self.ser = BT
    
    def front(self, dd):
        totalx, totaly = 0, 0
        d = 0
        self.ser.SerialWrite("f")
        print("Straight Foward!")
        while(True):
            x,y = pag.position() #獲取
            pag.moveTo(1100,self.screenHeight/2)
            totalx += self.screenHeight/2 - y
            totaly += 1100 - x #正軸往右
            d = (totalx**2 + totaly**2)**(1/2)
            #msg = self.ser.SerialReadString()
            print(d)
            if d >= dd: #or msg == "p":
                break
            time.sleep(0.0001)
        self.ser.SerialWrite("t")
        print("Final result: ", d)

if __name__ == "__main__":
    bt = BT.bluetooth()
    bt.connect_prog()
    m = Mouse(bt)
    print("press any key to start...")
    msg = input()
    bt.SerialWrite("a")
    while(True):
        if(bt.SerialReadString() == "f"):
            m.front(10000)