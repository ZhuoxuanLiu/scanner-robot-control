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
    print("进程被终止")


# 设置相应信号处理的handler
signal.signal(signal.SIGINT, sig_handler)
signal.signal(signal.SIGHUP, sig_handler)
signal.signal(signal.SIGTERM, sig_handler)


class Robot:
    def __init__(self) -> None:
        # 初始化系统状态
        self.sys_state = SysStat()
        # 初始化串口控制引擎
        self.eng = Engine(self.sys_state)
        # 初始化照相机
        self.cam = Camera(self.sys_state, './data')
    
    
    def start(self):
        while True:
            # 进书
            self.eng.in_book()
            # 不是最后一页
            while not self.sys_state.get_last_page_stat():
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
        Thread(target=self.start).start()
    
    
    def stop(self):
        # 停止机构
        self.eng.stop()


if __name__ == '__main__':
    robot = Robot()
    robot.t_start()
    while True:
        time.sleep(1)
        if stop:
            robot.stop()
            break