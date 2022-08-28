from copy import deepcopy
import serial#导入串口通信库
import time
from threading import Thread
from queue import Queue
from protocol import Protocol


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
                data = self.ser.read(4)
                # data = self.open_com.read() # read 1 size
                data = str(data)
                if data != '':
                    print(f'Get data is:{data}')
                    break
            self.data_queue.put(data)
            time.sleep(0.01)
    
    def t_read(self, over_time=1):
        Thread(target=self.read, args=(over_time,), daemon=True).start()
    
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
    
    logic_map = {
        Protocol.forward: Protocol.not_reseted,
        Protocol.backward: Protocol.reseted,
    }
    
    direction_map = {
        Protocol.forward: 'forward',
        Protocol.backward: 'backward',
    }
    
    def __init__(self, sys_state) -> None:
        self.sys_state = sys_state
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
        self.reset_answer_queue = deepcopy(queue_dict)
        self.t_serial_handler()
    
    @property
    def state(self):
        return self.sys_state
    
    def _recv_motor_command(self, motor, timeout):
        try:
            data = self.command_result_queue[motor].get(timeout=timeout)
            head, mode = data[1], data[2]
            self.sys_state.update_position(head, mode)
        except Exception as e:
            print('motor command recv timeout')
            self.sys_state.params['malfunction'] = True
            
    def _recv_pump_command(self, power, timeout):
        try:
            data = self.command_result_queue[Protocol.vacuum_pump].get(timeout=timeout)
            mode = data[2]
            if power == Protocol.on:
                if mode != Protocol.on:
                    raise Exception('pump command recv error response')
                self.sys_state.update_power(Protocol.vacuum_pump, Protocol.on)
            if power == Protocol.off:
                if mode != Protocol.off:
                    raise Exception('pump command recv error response')
                self.sys_state.update_power(Protocol.vacuum_pump, Protocol.off)
        except Exception as e:
            print(e)
            self.sys_state.params['malfunction'] = True
            
    def _recv_motor_check(self, motor, timeout):
        try:
            data = self.check_feedback_queue[motor].get(timeout=timeout)
            head, mode, body = data[1], data[2], data[3]
            if mode == Protocol.position:
                if body != self.sys_state.get_position(head):
                    raise Exception(f'state of {self.motor_map[motor]} doesn\'t match with system state')
                self.sys_state.update_position(head, body)
        except Exception as e:
            print(e)
            print(f'reset {self.motor_map[motor]} to system state')
            self.motor_reset(motor, timeout)
            
    def _recv_motor_reset(self, motor, timeout):
        try:
            data = self.reset_answer_queue[motor].get(timeout=timeout)
            head, mode = data[1], data[2]
            if mode != self.sys_state.get_position(head):
                raise Exception(f'state of {self.motor_map[motor]} doesn\'t match with system state')
        except Exception as e:
            print(e)
            self.sys_state.params['malfunction'] = True
    
    def motor_command(self, motor, direction, timeout, per=Protocol.hundred):
        try:
            if direction == Protocol.forward or direction == Protocol.backward:
                self.com.send(Protocol.command + motor + direction + per)
                self._recv_motor_command(motor, timeout)
                if self.sys_state.get_position(motor) != self.logic_map[direction]:
                    raise Exception(self.motor_map[motor] + ' can not ' + self.direction_map[direction])
            else:
                pass
        except Exception as e:
            print(e)
            self.sc.state.params['malfunction'] = True  
        
    def motor_command_multi(self, motor: tuple, direction: tuple, timeout, per: tuple):
        try:
            for m, d, p in zip(motor, direction, per):
                if d == Protocol.forward or d == Protocol.backward:
                    self.com.send(Protocol.command + m + d + p)
                else:
                    pass
            for m in motor:
                self._recv_motor_command(m, timeout)
                if self.sys_state.get_position(m) != self.logic_map[d]:
                    raise Exception(self.motor_map[m] + ' can not ' + self.direction_map[d])
        except Exception as e:
            print(e)
            self.sc.state.params['malfunction'] = True  
            
    def motor_command_rollback_all(self, timeout, exclude: tuple=None):
        try:
            for m in self.motor_list:
                if m not in exclude and self.sys_state.get_position(m) == Protocol.not_reseted:
                    self.com.send(Protocol.command + m + Protocol.backward + Protocol.hundred)
            for m in self.motor_list:
                if m not in exclude and self.sys_state.get_position(m) == Protocol.not_reseted:
                    self._recv_motor_command(m, timeout)
                    if self.sys_state.get_position(m) != Protocol.reseted:
                        raise Exception(self.motor_map[m] + ' can not reset')
        except Exception as e:
            print(e)
            self.sc.state.params['malfunction'] = True  
        
    def pump_command(self, power, timeout):
        if power == Protocol.on or power == Protocol.off:
            self.com.send(Protocol.command + Protocol.vacuum_pump + power + Protocol.bak)
            self._recv_pump_command(power, timeout)
        else:
            pass
        
    def motor_check(self, motor, timeout):
        self.com.send(Protocol.check + motor + Protocol.position + Protocol.bak)
        self._recv_motor_check(motor, timeout)
        
    def motor_check_all(self, timeout):
        for m in self.motor_list:
            self.com.send(Protocol.check + m + Protocol.position + Protocol.bak)
        for m in self.motor_list:
            self._recv_motor_check(m, timeout)
            
    def motor_reset(self, motor, timeout):
        self.com.send(Protocol.reset + motor + self.sys_state.get_position(motor) + Protocol.bak)
        self._recv_motor_reset(motor, timeout)
            
    def motor_reset_all(self, timeout):
        for m in self.motor_list:
            self.com.send(Protocol.reset + m + self.sys_state.get_position(m) + Protocol.bak)
        for m in self.motor_list:
            self._recv_motor_reset(m, timeout)
        
    def serial_handler(self):
        while True:
            if not self.com.data_queue.empty():
                data = self.com.data_queue.get()
                type, head = data[0], data[1]
                if type == Protocol.feedback:   #为简化 目前只处理电机位置和真空泵开关
                    self.check_feedback_queue[head].put(data)
                if type == Protocol.answer:
                    self.reset_answer_queue[head].put(data)
                if type == Protocol.result:
                    self.command_result_queue[head].put(data)
            else:
                time.sleep(0.1)
            
    def t_serial_handler(self):
        Thread(target=self.serial_handler, daemon=True).start()


class Engine:
    
    def __init__(self, sys_state) -> None:
        self.sc = SerialControl(sys_state)
        self.initial_check()
        pass
    
    
    def initial_check(self):
        assert self.sc.state.get_power(Protocol.vacuum_pump) == Protocol.off
        self.sc.motor_check_all(timeout=5)
                
    
    def activate_vacuum_pump(self, timeout=3):
        self.sc.pump_command(Protocol.on, timeout)
          
    
    def deactivate_vacuum_pump(self, timeout=3):
        self.sc.pump_command(Protocol.off, timeout)
        

    def motor_rotate(self, motor, per=Protocol.hundred, timeout=10):
        if self.sc.state.get_position(motor) == Protocol.reseted:
            self.sc.motor_command(motor, Protocol.forward, timeout, per)



    def motor_rotate_multi(self, motor: tuple, timeout=10):
        rotate_m = []
        rotate_d = []
        rotate_p = []
        for m in motor:
            if self.sc.state.get_position(m) == Protocol.reseted:
                rotate_m.append(m)
                rotate_d.append(Protocol.forward)
                rotate_p.append(Protocol.hundred)
        if rotate_m:
            self.sc.motor_command_multi(tuple(rotate_m), tuple(rotate_d), timeout, tuple(rotate_p))
                
                
    def motor_rollback(self, motor, per=Protocol.hundred, timeout=10):
        if self.sc.state.get_position(motor) == Protocol.not_reseted:
            self.sc.motor_command(motor, Protocol.backward, timeout, per)

    
    def rotate(self):
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rollback(Protocol.base_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.activate_vacuum_pump()
        if self.sc.state.params['malfunction'] == True:
            return
        # self.motor_rotate_multi((Protocol.head_motor, Protocol.base_motor))
        self.motor_rotate(Protocol.head_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rotate(Protocol.base_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rotate(Protocol.forward_pressing_board_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.deactivate_vacuum_pump()
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rotate(Protocol.body_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rollback(Protocol.head_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rotate(Protocol.pressing_board_motor)
        
        
    def reset(self):
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rollback(Protocol.body_motor)
        if self.sc.state.params['malfunction'] == True:
            return
        self.motor_rollback(Protocol.base_motor)
 
        
    def in_book(self):
        if self.sc.state.get_book_stat() == Protocol.off_board:
            if self.sc.state.params['malfunction'] == True:
                return
            self.motor_rotate(Protocol.lift_motor)
            if self.sc.state.params['malfunction'] == True:
                return
            self.motor_rotate(Protocol.pushing_book_motor)
            if self.sc.state.params['malfunction'] == True:
                return
            self.motor_rollback(Protocol.pushing_book_motor)
            if self.sc.state.params['malfunction'] == True:
                return
            self.motor_rollback(Protocol.lift_motor)
            if self.sc.state.params['malfunction'] == True:
                return
            self.sc.state.update_book_stat(Protocol.on_board)
            
        
    def out_book(self):
        if self.sc.state.get_book_stat() == Protocol.on_board:
            if self.sc.state.params['malfunction'] == True:
                return
            self.motor_rotate(Protocol.rotating_shelf_motor)
            if self.sc.state.params['malfunction'] == True:
                return
            self.motor_rollback(Protocol.rotating_shelf_motor)
            if self.sc.state.params['malfunction'] == True:
                return
            self.sc.state.update_book_stat(Protocol.off_board)
        
        
    def stop(self):
        self.sc.motor_command_rollback_all(exclude=(Protocol.base_motor,), timeout=20)
        self.motor_rotate(Protocol.base_motor, Protocol.fifty)  # 转到正中间
        self.deactivate_vacuum_pump()