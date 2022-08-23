from copy import deepcopy
import serial#导入串口通信库
import time
from threading import Thread
from queue import Queue
from protocol import Protocol
from sys_stat import SysStat


class SerialConnect:
    def __init__(self, port="/dev/ttyAMA0") -> None:
        self.ser = serial.Serial(
            port=port,   #设置串口号
            baudrate=9600,     # 波特率
            timeout=0.1,      # 读超时设置
            bytesize=8,   # 数据位数
            parity='N',   # 奇偶校验位
            stopbits=1, # 停止位
        )
        self.open()
        self.data_queue = Queue()
        self.t_read()

    def open(self):#对串口的参数进行配置
        self.ser.open()
        if(self.ser.isOpen()):
            print("串口打开成功！")
        else:
            print("串口打开失败！")
    #isOpen()函数来查看串口的开闭状态


    def close(self):
        self.ser.close()
        if(self.ser.isOpen()):
            print("串口关闭失败！")
        else:
            print("串口关闭成功！")


    def send(self, send_data):
        if(self.ser.isOpen()):
            self.ser.write(send_data.encode('utf-8'))#编码
            print("发送成功",send_data)
        else:
            print("发送失败！")


    def read(self, over_time):
        if not self.ser.isOpen():
            self.open()
        while True:
            start_time = time.time()
            while time.time() - start_time < over_time:
                data = self.ser.read(self.ser.inWaiting())
                # data = self.open_com.read() # read 1 size
                data = str(data)
                if data != '':
                    print(f'Get data is:{data}')
                    break
            self.data_queue.put(data)
    
    def t_read(self, over_time=1):
        Thread(target=self.read, args=(over_time,)).start()
    
    def __del__(self):
        self.close()
        
        
class SerialControl:
    
    def __init__(self) -> None:
        self.sys_state = SysStat()
        self.com = SerialConnect()
        queue_dict = {
            Protocol.base_motor: Queue(),
            Protocol.body_motor: Queue(),
            Protocol.head_motor: Queue(),
            Protocol.lift_motor: Queue(),
            Protocol.pushing_book_motor: Queue(),
            Protocol.forward_pressing_board_motor: Queue(),
            Protocol.pressing_board_motor: Queue(),
            Protocol.rotating_shelf_motor: Queue(),
            Protocol.vacuum_pump: Queue(),
        }
        self.check_feedback_queue = queue_dict
        self.command_result_queue = deepcopy(queue_dict)
        self.t_serial_handler()
    
    @property
    def state(self):
        return self.sys_state
    
    def _recv_motor_command(self, motor, timeout):
        try:
            data = self.command_result_queue[motor].get(timeout=timeout)
            head, body = data[1], data[2]
            self.sys_state.update_position(head, body)
        except Exception as e:
            self.sys_state.params['malfunction'] = True
            
    def _recv_pump_command(self, power, timeout):
        try:
            data = self.command_result_queue[Protocol.vacuum_pump].get(timeout=timeout)
            body = data[2]
            if power == Protocol.on:
                if body != Protocol.on:
                    raise Exception('error response')
                self.sys_state.update_power(Protocol.vacuum_pump, Protocol.on)
            if power == Protocol.off:
                if body != Protocol.off:
                    raise Exception('error response')
                self.sys_state.update_power(Protocol.vacuum_pump, Protocol.off)
        except Exception as e:
            self.sys_state.params['malfunction'] = True
            
    def _recv_motor_check(self, motor, timeout, mode):
        try:
            data = self.check_feedback_queue[motor].get(timeout=timeout)
            head, body = data[1], data[2]
            if mode == Protocol.position:
                self.sys_state.update_position(head, body)
            if mode == Protocol.power:
                self.sys_state.update_power(head, body)
        except Exception as e:
            self.sys_state.params['malfunction'] = True
    
    def motor_command(self, motor, direction, timeout):
        if direction == Protocol.forward or direction == Protocol.backward:
            self.com.send(Protocol.command + motor + direction)
            self._recv_motor_command(motor, timeout)
        else:
            pass
        
    def pump_command(self, power, timeout):
        if power == Protocol.on or power == Protocol.off:
            self.com.send(Protocol.command + Protocol.vacuum_pump + power)
            self._recv_pump_command(power, timeout)
        else:
            pass
        
    def motor_check(self, motor, timeout, mode=Protocol.position):
        if mode == Protocol.position or mode == Protocol.power:
            self.com.send(Protocol.check + motor + mode)
            self._recv_motor_check(motor, timeout, mode)
        else:
            pass
        
    def serial_handler(self):
        while True:
            if not self.com.data_queue.empty():
                data = self.com.data_queue.get()
                type, head, body = data[0], data[1], data[2]
                if type == Protocol.query:
                    if body == Protocol.position:
                        motor_pos = self.sys_state.get_position(head)
                        self.com.send(Protocol.feedback + head + motor_pos)
                if type == Protocol.feedback:   #为简化 目前只处理电机位置和真空泵开关
                    self.check_feedback_queue[head].put(data)
                if type == Protocol.result:
                    self.command_result_queue[head].put(data)
            
    def t_serial_handler(self):
        Thread(target=self.serial_handler).start()


class Move:
    
    def __init__(self) -> None:
        self.sc = SerialControl()
        pass
    
    
    def initial_check(self):
        assert self.sc.state.get_power(Protocol.vacuum_pump) == Protocol.off
        
    
    def base_motor_reset(self, timeout=10):
        if self.sc.state.get_position(Protocol.base_motor) != Protocol.reseted:
            try:
                self.sc.motor_command(Protocol.base_motor, Protocol.backward, timeout)
                if self.sc.state.get_position(Protocol.base_motor) != Protocol.reseted:
                    raise Exception('base motor can not reset')
            except Exception as e:
                self.sc.state.params['malfunction'] = True
                
    
    def activate_vacuum_pump(self, timeout=3):
        self.sc.pump_command(Protocol.on, timeout)
        
    
    def rotate_head_motor(self, direction, timeout=3):
        try:
            self.sc.motor_command(Protocol.head_motor, direction, timeout)
            if self.sc.state.get_position(Protocol.head_motor) != Protocol.not_reseted:
                raise Exception('head motor can not rotate')
        except Exception as e:
            self.sc.state.params['malfunction'] = True
            
            
    def base_motor_rotate(self, timeout=10):
        if self.sc.state.get_position(Protocol.base_motor) == Protocol.reseted:
            try:
                self.sc.motor_command(Protocol.base_motor, Protocol.forward, timeout)
                if self.sc.state.get_position(Protocol.base_motor) != Protocol.not_reseted:
                    raise Exception('base motor can not rotate')
            except Exception as e:
                self.sc.state.params['malfunction'] = True
