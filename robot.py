from protocol import Protocol
from utils import Engine

class Robot:
    def __init__(self) -> None:
        self.eng = Engine()
        pass
    
    def step(self):
        pass
    
    def start(self):
        self.eng.in_book()
        while last_page_sign:
            self.eng.move()
        # do something to know whether it is the last page
        self.eng.out_book()
        pass
    
    def stop(self):
        pass
    
    def reset(self):
        pass