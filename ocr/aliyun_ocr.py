# -*- coding: utf-8 -*-
# This file is auto-generated, don't edit it. Thanks.
import json
from xhtml2pdf import pisa

import os
import oss2
# pip install oss2

from Tea.core import TeaCore
from alibabacloud_ocr20191230.client import Client as ocr20191230Client
from alibabacloud_tea_openapi import models as open_api_models
from alibabacloud_ocr20191230 import models as ocr_20191230_models
from alibabacloud_tea_util import models as util_models
from alibabacloud_tea_util.client import Client as UtilClient
# pip install alibabacloud_ocr20191230==2.0.10

class OCR:
    def __init__(self):
        self.client = self.create_client('LTAI5t9pZMs5AGpxNMJt4jt4', '69Zx3TBKFKGIMJfJWhL3BDi3Py01hi')
        pass

    def create_client(
        self,
        access_key_id: str,
        access_key_secret: str,
    ) -> ocr20191230Client:
        """
        使用AK&SK初始化账号Client
        @param access_key_id:
        @param access_key_secret:
        @return: Client
        @throws Exception
        """
        config = open_api_models.Config(
            # 您的 AccessKey ID,
            access_key_id=access_key_id,
            # 您的 AccessKey Secret,
            access_key_secret=access_key_secret
        )
        # 访问的域名
        config.endpoint = f'ocr.cn-shanghai.aliyuncs.com'
        return ocr20191230Client(config)

    def process(
        self,
        object_name,
        object_path
    ):
        
        # 首先初始化AccessKeyId、AccessKeySecret、Endpoint等信息。
        # 通过环境变量获取，或者把诸如“<你的AccessKeyId>”替换成真实的AccessKeyId等。

        # 分别以HTTP、HTTPS协议访问。
        access_key_id = os.getenv('OSS_TEST_ACCESS_KEY_ID', 'LTAI5t9pZMs5AGpxNMJt4jt4')
        access_key_secret = os.getenv('OSS_TEST_ACCESS_KEY_SECRET', '69Zx3TBKFKGIMJfJWhL3BDi3Py01hi')
        bucket_name = os.getenv('OSS_TEST_BUCKET', 'scanner-lzx')
        endpoint = os.getenv('OSS_TEST_ENDPOINT', 'oss-cn-shanghai.aliyuncs.com')
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

        trim_document_request = ocr_20191230_models.TrimDocumentRequest(
            file_url=url,   # must be oss
            file_type='pdf',
            output_type='html'
        )
        runtime = util_models.RuntimeOptions()
        try:
            res_id = self.client.trim_document_with_options(trim_document_request, runtime).body.request_id
            get_async_job_result_request = ocr_20191230_models.GetAsyncJobResultRequest(job_id=res_id)
            runtime = util_models.RuntimeOptions()
            try:
                ret_json = json.loads(UtilClient.to_jsonstring(TeaCore.to_map(self.client.get_async_job_result_with_options(get_async_job_result_request, runtime))))
                res_status = ret_json['body']['Data']['Status']
                while (res_status != 'PROCESS_SUCCESS'):
                    ret_json = json.loads(UtilClient.to_jsonstring(TeaCore.to_map(self.client.get_async_job_result_with_options(get_async_job_result_request, runtime))))
                    res_status = ret_json['body']['Data']['Status']
                    

                ret_html = json.loads(ret_json['body']['Data']['Result'])['Content'] + "<style> body { font-family: STSong-Light } </style>"
                resultFile = open(r'C:\Users\a3352\Desktop\out.pdf', "w+b")
                pisa.CreatePDF(ret_html, resultFile)
                resultFile.close()

            except Exception as error:
                # 如有需要，请打印 error
                UtilClient.assert_as_string(error)
        except Exception as error:
            # 如有需要，请打印 error
            UtilClient.assert_as_string(error)


if __name__ == '__main__':
    ocr = OCR()
    ocr.process('test1.pdf', r'C:\Users\a3352\Desktop\result1.pdf')
