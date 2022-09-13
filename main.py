from threading import Thread
from utils.serialUtils import Engine
from sysStat import SysStat
from utils.cvUtils import Camera
import time
import signal

stop = False

# 自定义信号处理函数
def sig_handler(signum, frame):
    global stop
    stop = True
    print("process terminated")

def bind_signal():
# 设置相应信号处理的handler
    signal.signal(signal.SIGINT, sig_handler)
    # signal.signal(signal.SIGHUP, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)


class Robot:
    def __init__(self) -> None:
        # 初始化系统状态
        self.sys_state = SysStat()
        # 初始化串口控制引擎
        self.eng = Engine(self.sys_state)
        # 初始化照相机
        self.cam = Camera(self.sys_state, './data')
        self.t_detect_malfunction()
        self.command_dict = {
            '1': self.eng.initial_check,
            '2': self.eng.activate_vacuum_pump,
            '3': self.eng.deactivate_vacuum_pump,
            '4': self.eng.rotate_revised,
            '5': self.cam.capture,
            '6': self.eng.whole_process,
            '7': self.eng.in_out_demo,
        }
    
    
    def start(self):
        global stop
        while not stop:
            # 进书
            self.eng.in_book()
            # 不是最后一页
            while not (stop and self.sys_state.get_last_page_stat()):
                # 翻页
                self.eng.rotate()
                # 拍照
                self.cam.capture()
                # 复位
                self.eng.reset()
            # 出书
            self.eng.out_book()
            time.sleep(1)


    def t_start(self):
        # 运行线程
        Thread(target=self.start, daemon=True).start()
        

    # 检测到故障 停机
    def detect_malfunction(self):
        while True:
            if self.sys_state.params['malfunction'] == True:
                global stop
                stop = True
                break
            time.sleep(0.1)
            
    
    def t_detect_malfunction(self):
        Thread(target=self.detect_malfunction, daemon=True).start()
    
    
    def stop(self):
        # 停止机构
        self.eng.stop()


if __name__ == '__main__':
    command = input('[1] start \n[2] quit \nplease input:')
    if command == '1':
        print('robot started!')
        robot = Robot()
        bind_signal()
        while True:
            command = input('[1] initial check \n[2] activate pump \n[3] deactivate pump \
                                \n[4] flip single move \n[5] camera test \n[6] whole process\n\
                                [7] in out whole \n[q] quit \nplease input:')
            if command not in ['1', '2', '3', '4', '5', '6', '7']:
                print('---robot stopped---')
                break 
            robot.command_dict[command]()
            if stop:
                # robot.stop()
                print('---robot stopped---')
                break1
                11111
    elif command == '2':
        print('quit!')
    else:
        print('wrong command!')
        