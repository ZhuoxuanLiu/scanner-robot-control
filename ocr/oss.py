import os
import shutil

import oss2
# pip install oss2

# 首先初始化AccessKeyId、AccessKeySecret、Endpoint等信息。
# 通过环境变量获取，或者把诸如“<你的AccessKeyId>”替换成真实的AccessKeyId等。
#
# 以杭州区域为例，Endpoint可以是：
#   http://oss-cn-hangzhou.aliyuncs.com
#   https://oss-cn-hangzhou.aliyuncs.com
# 分别以HTTP、HTTPS协议访问。
access_key_id = os.getenv('OSS_TEST_ACCESS_KEY_ID', 'LTAI5t9pZMs5AGpxNMJt4jt4')
access_key_secret = os.getenv('OSS_TEST_ACCESS_KEY_SECRET', '69Zx3TBKFKGIMJfJWhL3BDi3Py01hi')
bucket_name = os.getenv('OSS_TEST_BUCKET', 'scanner-lzx')
endpoint = os.getenv('OSS_TEST_ENDPOINT', 'oss-cn-shanghai.aliyuncs.com')


def upload(object_name, object_path):
    # 创建Bucket对象，所有Object相关的接口都可以通过Bucket对象来进行
    bucket = oss2.Bucket(oss2.Auth(access_key_id, access_key_secret), endpoint, bucket_name)
    bucket.put_object_from_file(object_name, object_path)
    # 指定Header。
    headers = dict()
    # 如果您希望实现浏览器访问时自动下载文件，并自定义下载后的文件名称，配置文件HTTP头中的Content-Disposition为attachment
    # headers['content-disposition'] = 'attachment'
    # 如果您希望直接在浏览器中预览文件，配置文件HTTP头中的Content-Disposition为inline并使用Bucket绑定的自定义域名进行访问。
    headers['content-disposition'] = 'inline'
    url = bucket.sign_url('GET', object_name, 60, headers=headers, slash_safe=True)
    return url
    # return "https://" + bucket_name + "." + endpoint + "/" + object_name


file_url = upload('test1.pdf', r'C:\Users\a3352\Desktop\result1.pdf')
print(file_url)
