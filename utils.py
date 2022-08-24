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
    
    motor_list = (Protocol.base_motor, Protocol.body_motor, Protocol.head_motor, Protocol.lift_motor,
                  Protocol.pushing_book_motor, Protocol.forward_pressing_board_motor, Protocol.pressing_board_motor,
                  Protocol.rotating_shelf_motor)
    
    motor_map = {
        Protocol.base_motor: 'base motor',
        Protocol.body_motor: 'body motor',
        Protocol.head_motor: 'head motor',
        Protocol.lift_motor: 'lift motor',
        Protocol.pushing_book_motor: 'pushing book motor',
        Protocol.forward_pressing_board_motor: 'forward pressing board motor',
        Protocol.pressing_board_motor: 'pressing board motor',
        Protocol.rotating_shelf_motor: 'rotating shelf motor',
    }
    
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
        
    def motor_command_multi(self, motor: tuple, direction: tuple, timeout):
        for m, d in zip(motor, direction):
            if d == Protocol.forward or d == Protocol.backward:
                self.com.send(Protocol.command + m + d)
            else:
                pass
        for m in motor:
            self._recv_motor_command(m, timeout)
            
    def motor_command_reset_all(self, timeout, exclude: tuple=None):
        for m in self.motor_list:
            if m not in exclude:
                self.com.send(Protocol.command + m + Protocol.backward)
        for m in self.motor_list:
            if m not in exclude:
                self._recv_motor_command(m, timeout)
        
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
        
    def motor_check_all(self, timeout, mode=Protocol.position):
        for m in self.motor_list:
            if mode == Protocol.position or mode == Protocol.power:
                self.com.send(Protocol.check + m + mode)
            else:
                pass
        for m in self.motor_list:
            self._recv_motor_check(m, timeout, mode)
        
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


class Engine:
    
    def __init__(self) -> None:
        self.sc = SerialControl()
        self.initial_check()
        pass
    
    
    def initial_check(self):
        assert self.sc.state.get_power(Protocol.vacuum_pump) == Protocol.off
                
    
    def activate_vacuum_pump(self, timeout=3):
        self.sc.pump_command(Protocol.on, timeout)
          
    
    def deactivate_vacuum_pump(self, timeout=3):
        self.sc.pump_command(Protocol.off, timeout)
        

    def motor_rotate(self, motor, timeout=10):
        if self.sc.state.get_position(motor) == Protocol.reseted:
            try:
                self.sc.motor_command(motor, Protocol.forward, timeout)
                if self.sc.state.get_position(motor) != Protocol.not_reseted:
                    raise Exception(self.motor_map[motor] + ' can not rotate')
            except Exception as e:
                self.sc.state.params['malfunction'] = True  
                
                
    def motor_reset(self, motor, timeout=10):
        if self.sc.state.get_position(motor) == Protocol.not_reseted:
            try:
                self.sc.motor_command(motor, Protocol.backward, timeout)
                if self.sc.state.get_position(motor) != Protocol.reseted:
                    raise Exception(self.motor_map[motor] + ' can not reset')
            except Exception as e:
                self.sc.state.params['malfunction'] = True  
    
                
    def move(self):
        self.motor_reset(Protocol.base_motor)
        self.activate_vacuum_pump()
        self.motor_rotate(Protocol.head_motor)
        self.motor_reset(Protocol.base_motor)
        self.motor_rotate(Protocol.forward_pressing_board_motor)
        self.deactivate_vacuum_pump()
        self.motor_reset(Protocol.head_motor)
        self.motor_rotate(Protocol.body_motor)
        self.motor_rotate(Protocol.pressing_board_motor)
        self.motor_reset(Protocol.body_motor)
        self.motor_reset(Protocol.base_motor)
        
        
    def in_book(self):
        if self.sc.state.get_book_stat() == Protocol.off_board:
            self.motor_rotate(Protocol.lift_motor)
            self.motor_rotate(Protocol.pushing_book_motor)
            self.motor_reset(Protocol.pushing_book_motor)
            self.motor_reset(Protocol.lift_motor)
            self.sc.state.update_book_stat(Protocol.on_board)
            
        
    def out_book(self):
        self.motor_rotate(Protocol.rotating_shelf_motor)
        self.motor_reset(Protocol.rotating_shelf_motor)
        
        
    def stop(self):
        self.sc.motor_command_reset_all(exclude=(Protocol.base_motor,), timeout=20)
        self.deactivate_vacuum_pump()