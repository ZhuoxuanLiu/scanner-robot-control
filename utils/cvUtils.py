from threading import Thread
import cv2
from pyzbar.pyzbar import decode
from PIL import Image
import qrcode
import os
import requests
import json
import base64
from queue import Queue
import time
from picamera import PiCamera


class cvUtils:
    
    @staticmethod
    def binaryzation(img_path, save_path):
        img = cv2.imread(img_path, 0)
        # 1.全局阈值法
        _, mask_all = cv2.threshold(src=img,                  # 要二值化的图片
                                    thresh=127,               # 全局阈值
                                    maxval=255,               # 大于全局阈值后设定的值
                                    type=cv2.THRESH_BINARY)   # 设定的二值化类型，THRESH_BINARY：表示小于阈值置0，大于阈值置填充色
        img = Image.fromarray(mask_all)
        img.save(save_path)

    @staticmethod
    def generate_qrcode():
        # 调用QRCode，可以接收以下参数
        """
        version: 二维码的格子大小，可以是1到40。值越大，格子越大，一般不超过10，选择3比较合适
        error_correction: 二维码错误容许率，默认为qrcode.constants.ERROR_CORRECT_M，容许小于 15% 的错误率
                此外还有qrcode.constants.ERROR_CORRECT_L: 容许小于 7% 的错误率
                qrcode.constants.ROR_CORRECT_H: 容许小于 30% 的错误率
        box_size: 二维码每个小格子包含的像素数量
        border: 二维码到图片边框的小格子数，默认值为 4         
        """
        qr = qrcode.QRCode() # 事实上里面的参数我们可以都不指定，默认会选择一个比较合适的参数
        # 调用add_data，指定url。
        qr.add_data("scan robot")
        # 生成二维码图像，颜色为蓝色，背景色为粉色
        img = qr.make_image(fill_color='black', back_color='white')
        # 此外，我们还可以保存到硬盘上
        img.save("./data/qrcode.png")

    @staticmethod
    def detect_qrcode(img_path, sys_state) -> bool:
        img = Image.open(img_path)
        barcode = decode(img)
        for code in barcode:
            res = code.data.decode("utf-8")
            if res == 'scan robot':
                sys_state.update_last_page_stat(True)
                return True
        return False
    
    @staticmethod
    def img2pdf(images_path, pdf_path):
        files = os.listdir(images_path)
        images = []
        sources = []
        for file in files:
            if 'png' in file or 'jpg' in file:
                images.append(images_path + file)
        images.sort(key=cvUtils._sort_key)
        output = Image.open(images[0])
        images.pop(0)
        for file in images:
            png_file = Image.open(file)
            sources.append(png_file)
        output.save(pdf_path, "pdf", save_all=True, append_images=sources)
    
    def _sort_key(s):
        if s:
            try:
                num = int(s.rpartition('/')[-1].rpartition('.')[0])
            except:
                num = -1
            return num
    
    
class Camera:
    
    def __init__(self, sys_state, save_path, resolution=(1920, 1080)) -> None:
        self.cam = PiCamera()
        self.sys_state = sys_state
        self.save_path = save_path
        self.cam.resolution = resolution
        self.img_process_queue = Queue()
        # 请登录后前往 “工作台-账号设置-开发者信息” 查看 x-ti-app-id
        # 示例代码中 x-ti-app-id 非真实数据
        self._app_id = '12b7eb61e04fc9d07c6c5ce8fd240146'
        # 请登录后前往 “工作台-账号设置-开发者信息” 查看 x-ti-secret-code
        # 示例代码中 x-ti-secret-code 非真实数据
        self._secret_code = '5d9d24a940050b271395d636b2a5f5d6'
        if not os.path.exists(save_path + '/pdf/'):
            os.makedirs(save_path + '/pdf/')
        if not os.path.exists(save_path + '/oringin/'):
            os.makedirs(save_path + '/oringin/')
        if not os.path.exists(save_path + '/bi/'):
            os.makedirs(save_path + '/bi/')
        self.t_process(bi=False)


    @property
    def page_num(self):
        return self.sys_state.get_page_num()
    
    
    def page_num_plus1(self):
        self.sys_state.update_page_num(self.page_num + 1)
    
    
    def cut(self, img_path, last_page):
        img = Image.open(img_path)
        img_size = img.size
        h = img_size[1]  # 图片高度
        w = img_size[0]  # 图片宽度
        left = img.crop((0, 0, w / 2, h))    # crop函数里面四个数据分别为左、上、右、下，按照这个规则来分割你想的比例图片。
        self.page_num_plus1()
        left_path = img_path.rpartition('/')[0] + '/' + str(self.page_num) + '.png'
        left_name = 'page' + str(self.page_num) + '.png'
        left.save(left_path)
        if not last_page:
            right = img.crop((w / 2, 0, w, h))
            self.page_num_plus1()
            right_path = img_path.rpartition('/')[0] + '/' + str(self.page_num) + '.png'
            right_name = 'page' + str(self.page_num) + '.png'
            right.save(right_path)
        os.remove(img_path)
        return (left_name, right_name if not last_page else None)
    
    
    def capture(self):
        img_path = self.save_path + '/oringin/page.png'
        self.cam.capture(img_path)
        self.img_process_queue.put(img_path)
        
    
    def recognize(self, img_path, save_path):
        # 图片切边增强
        url = 'https://api.textin.com/ai/service/v1/crop_enhance_image'
        head = {}
        try:
            with open(img_path, 'rb') as fp:
                img = fp.read()
            head['x-ti-app-id'] = self._app_id
            head['x-ti-secret-code'] = self._secret_code
            result = requests.post(url, data=img, headers=head)
            image_str = json.loads(result.text)['result']['image_list'][0]['image']
            imgdata = base64.b64decode(image_str)
            #将图片保存为文件
            with open(save_path,'wb') as f:
                f.write(imgdata)
        except Exception as e:
            print(e)


    def process(self, bi):
        while True:
            if not self.img_process_queue.empty():
                img_path = self.img_process_queue.get()
                self.recognize(img_path, img_path)
                images = self.cut(img_path, last_page=cvUtils.detect_qrcode(img_path, self.sys_state))
                if bi:
                    for img in images:
                        img_path = self.save_path + '/oringin/' + img
                        save_path = self.save_path + '/bi/' + img
                        cvUtils.binaryzation(img_path, save_path)
                if self.sys_state.get_last_page_stat():
                    if bi:
                        cvUtils.img2pdf(self.save_path + '/bi/', self.save_path + '/pdf/' + 'output.pdf')
                    else:
                        cvUtils.img2pdf(self.save_path + '/oringin/', self.save_path + '/pdf/' + 'output.pdf')
            else:
                time.sleep(0.5)
                
                
    def t_process(self, bi=True):
        Thread(target=self.process, args=(bi,), daemon=True).start()
    
    

    

 

            

