# 以下变量为协议设置


class Protocol:
    # send protocol
    check = '0'
    reset = '1'
    command = '2'
    
    base_motor = '0'
    body_motor = '1'
    head_motor = '2'
    lift_motor = '3'
    pushing_book_motor = '4'
    forward_pressing_board_motor = '5'
    pressing_board_motor = '6'
    rotating_shelf_motor = '7'
    vacuum_pump = '8'
    
    position = '0'
    power = '1'
    
    not_reseted = '0'
    reseted = '1'
    
    forward = '1'
    backward = '0'
    
    on = '1'
    off = '0'
    
    # recieve protocol
    feedback = '0'
    answer = '1'
    result = '2'
    
    # book on board or not
    off_board = '0'
    on_board = '1'
    
    # percentage
    ten = '1'
    twenty = '2'
    thirty = '3'
    forty = '4'
    fifty = '5'
    sixty = '6'
    seventy = '7'
    ninety = '9'
    hundred = 'x'
    
    bak = '0'


