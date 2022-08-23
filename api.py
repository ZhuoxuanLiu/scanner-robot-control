import requests
import json
import base64

def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()

class Img_transform(object):
    def __init__(self, img_path):
        # 请登录后前往 “工作台-账号设置-开发者信息” 查看 x-ti-app-id
        # 示例代码中 x-ti-app-id 非真实数据
        self._app_id = '12b7eb61e04fc9d07c6c5ce8fd240146'
        # 请登录后前往 “工作台-账号设置-开发者信息” 查看 x-ti-secret-code
        # 示例代码中 x-ti-secret-code 非真实数据
        self._secret_code = '5d9d24a940050b271395d636b2a5f5d6'
        self._img_path = img_path

    def recognize(self):
        # 图片切边增强
        url = 'https://api.textin.com/ai/service/v1/crop_enhance_image'
        head = {}
        try:
            image = get_file_content(self._img_path)
            head['x-ti-app-id'] = self._app_id
            head['x-ti-secret-code'] = self._secret_code
            result = requests.post(url, data=image, headers=head)
            image_str = json.loads(result.text)['result']['image_list'][0]['image']
            imgdata = base64.b64decode(image_str)
            #将图片保存为文件
            with open(r'C:\Users\a3352\Desktop\result1.jpg','wb') as f:
                f.write(imgdata)
        except Exception as e:
            return e


            
if __name__ == "__main__":
    trans = Img_transform(r'C:\Users\a3352\Desktop\test1.jpg')
    trans.recognize()



