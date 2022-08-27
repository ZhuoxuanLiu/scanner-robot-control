# 以下变量为协议设置


class Protocol:
    # send protocol
    check = '0'
    reset = '1'
    command = '2'
    
    # 机构标志位
    base_motor = '0'                        # 基座电机
    body_motor = '1'                        # 机械臂升降电机
    head_motor = '2'                        # 吸附头旋转电机
    lift_motor = '3'                        # 进书升降电机
    pushing_book_motor = '4'                # 推书电机
    forward_pressing_board_motor = '5'      # 压书机构前进电机
    pressing_board_motor = '6'              # 压书板下降电机
    rotating_shelf_motor = '7'              # 旋转书架电机
    vacuum_pump = '8'                       # 真空泵
    
    # 类型标志位
    position = '0'
    power = '1'
    
    # 是否复位
    not_reseted = '0'
    reseted = '1'
    
    # 旋转方向
    forward = '1'
    backward = '0'
    
    # 开关状态
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
    
    # 备用位
    bak = '0'


