from copy import deepcopy
import serial#导入串口通信库
import time
from threading import Thread
from queue import Queue


class SerialConnect:
    def __init__(self, port="/dev/ttyAMA0") -> None:
        self.ser = serial.Serial(
            port=port,   #设置串口号
            baudrate=115200,     # 波特率
            timeout=0.1,      # 读超时设置
            bytesize=8,   # 数据位数
            parity='N',   # 奇偶校验位
            stopbits=1, # 停止位
        )
        self.data_queue = Queue()


    def open(self):#对串口的参数进行配置
        if(self.ser.isOpen()):
            print("serial open success！")
        else:
            self.ser.open()
            if(self.ser.isOpen()):
                print("serial open success!")
            else:
                print("serial open failed!")
    #isOpen()函数来查看串口的开闭状态


    def close(self):
        self.ser.close()
        if(self.ser.isOpen()):
            print("serial close failed!")
        else:
            print("serial close success!")


    def send(self, send_data):
        if(self.ser.isOpen()):
            self.ser.write(send_data.encode())#编码
            print("send success",send_data)
        else:
            print("send failed")


    def read(self, over_time):
        if not self.ser.isOpen():
            self.open()
        while True:
            start_time = time.time()
            while time.time() - start_time < over_time:
                data = self.ser.read(4)
                # data = self.open_com.read() # read 1 size
                data = str(data.decode('utf-8', 'ignore').strip(b'\x00'.decode()))
                if data != '':
                    print(f'Get data is:{data}', len(data))
                    self.data_queue.put(data)
                    break
            time.sleep(0.01)
    
    def t_read(self, over_time=1):
        Thread(target=self.read, args=(over_time,), daemon=True).start()
    
    def __del__(self):
        self.close()

# '2315' '2305'
send_data='270a\r\n'
ser = SerialConnect()
ser.send(send_data)
ser.read(0.2)
