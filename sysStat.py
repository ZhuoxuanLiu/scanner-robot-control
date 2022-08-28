from protocol import Protocol
from database import Mongo

    
class SysStat:
    
    position = {
        Protocol.base_motor: Protocol.reseted,
        Protocol.body_motor: Protocol.reseted,
        Protocol.head_motor: Protocol.reseted,
        Protocol.lift_motor: Protocol.reseted,
        Protocol.pushing_book_motor: Protocol.reseted,
        Protocol.forward_pressing_board_motor: Protocol.reseted,
        Protocol.pressing_board_motor: Protocol.reseted,
        Protocol.rotating_shelf_motor: Protocol.reseted,
    }
    
    power = {
        Protocol.base_motor: Protocol.off,
        Protocol.body_motor: Protocol.off,
        Protocol.head_motor: Protocol.off,
        Protocol.lift_motor: Protocol.off,
        Protocol.pushing_book_motor: Protocol.off,
        Protocol.forward_pressing_board_motor: Protocol.off,
        Protocol.pressing_board_motor: Protocol.off,
        Protocol.rotating_shelf_motor: Protocol.off,
        Protocol.vacuum_pump: Protocol.off,
    }
    
    params = {
        'power': power,                     # 开关状态   
        'position': position,               # 位置状态
        'malfunction': False,               # 故障状态
        'book': Protocol.off_board,         # 书是否在机器上
        'last_page':False,                  # 是否是最后一页
        'page_num': 0,                      # 页码
    }
    
    def __init__(self) -> None:
        self.mongo = Mongo()
        self.params = self.mongo.get_document()
    
    
    def update_position(self, motor, position):
        self.position[motor] = position
        self.mongo.update_document(self.params)
        
        
    def get_position(self, motor):
        return self.position[motor]


    def update_power(self, param, stat):
        self.power[param] = stat
        self.mongo.update_document(self.params)
        
        
    def get_power(self, param):
        return self.power[param]
    
    
    def get_book_stat(self):
        return self.params['book']
    
    
    def update_book_stat(self, stat):
        self.params['book'] = stat
        self.mongo.update_document(self.params)


    def get_last_page_stat(self):
        return self.params['last_page']
    
    
    def update_last_page_stat(self, stat):
        self.params['last_page'] = stat
        self.mongo.update_document(self.params)
        
    
    def get_page_num(self):
        return self.params['page_num']
    
    
    def update_page_num(self, stat):
        self.params['page_num'] = stat
        self.mongo.update_document(self.params)
