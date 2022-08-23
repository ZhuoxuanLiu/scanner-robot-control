import numpy as np
from scipy import signal
from PIL import Image

# I = Image.open('./data/result1.jpg') 
# I.show()    
# I_array = np.array(I)


import cv2
import matplotlib.pyplot as plt
import matplotlib

matplotlib.rcParams['font.sans-serif'] = ['SimHei']     # 显示中文
# 为了坐标轴负号正常显示。matplotlib默认不支持中文，设置中文字体后，负号会显示异常。需要手动将坐标轴负号设为False才能正常显示负号。
matplotlib.rcParams['axes.unicode_minus'] = False


# 读取灰度图像
img = cv2.imread('./data/result1.jpg', 0)
plt.subplot(1, 2, 1)
plt.imshow(img, cmap='gray')
plt.title("原图")


# 1.全局阈值法
ret, mask_all = cv2.threshold(src=img,                  # 要二值化的图片
                              thresh=127,               # 全局阈值
                              maxval=255,               # 大于全局阈值后设定的值
                              type=cv2.THRESH_BINARY)   # 设定的二值化类型，THRESH_BINARY：表示小于阈值置0，大于阈值置填充色
print("全局阈值的shape: ", mask_all.shape)
plt.subplot(1, 2, 2)
plt.imshow(mask_all, cmap='gray')
plt.title("全局阈值")
plt.show()

