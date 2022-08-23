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
        'power': power,
        'position': position,
        'malfunction': False,
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

