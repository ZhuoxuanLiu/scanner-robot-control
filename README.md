# Scan Robot
 
python and stm32 source code

&emsp;
## Getting Started
 
These instructions will show how each part of the program is organized
&emsp;
 
### Prerequisites
 
python packages list
 
```
opencv-python
pyzbar
Pillow
qrcode
picamera
pymongo
```
&emsp;
## Running the python code
 
Personal Settings that need to be modified
 
### Configure mongodb

 In database.py, I set the account and password to:
```
MongoClient(username="raspberrypi", password="123456")
```
 Please change the password based on your own account password

 For more info, please check:
 * [Install MongoDB on Windows](https://www.mongodb.com/docs/manual/tutorial/install-mongodb-on-windows/)
* [Install MongoDB on Linux](https://www.mongodb.com/docs/manual/administration/install-on-linux/)
  
### Configure serial port
 
 In serialUtils.py, I set the serial port to:
 
```
port="/dev/ttyAMA0"
```
 Please change the port based on your own machine

&emsp;
 ## Code Framework
 ```
|-- data
|   |-- oringin                     拍照原始图片储存路径
|   |-- bi                          二值化处理后图片储存路径
|   `-- pdf                         生成pdf路径
|   
|-- document                        控制协议文档
|   `-- robot logic.docx
|   
|-- ocr                             阿里云ocr函数(效果不理想弃用)
|   `-- aliyu_ocr.py
|   
|-- stm_core                        单片机c程序源代码  
|   
|-- utils                           辅助类
|   |-- cvUtils.py                  图像处理类
|   `-- serialUtils.py              串口控制类
|   
|-- database.py                     mongo数据库交互
|   
|-- main.py                         启动文件
|   
|-- protocol.py                     串口通讯协议，请参照协议表
|    
`-- sysStat.py                      系统状态记录类
```
 
