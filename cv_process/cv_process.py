import cv2

import numpy as np

#读取照片并显示

image=cv2.imread(r'C:\Users\a3352\Desktop\scanner\data\result1.jpg')

image = cv2.resize(image,None,fx=0.5,fy=0.5)

rows,cols,channels = image.shape

hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
cv2.imshow('hsv', hsv)
print(hsv[0, 0])

# # 区间1
# lower_red = np.array([0, 43, 46])
# upper_red = np.array([10, 255, 255])
# mask0 = cv2.inRange(hsv,lower_red,upper_red)
# # 区间2
# lower_red = np.array([156, 43, 46])
# upper_red = np.array([180, 255, 255])
# mask1 = cv2.inRange(hsv,lower_red,upper_red)
# 拼接两个区间
# mask = mask0 + mask1
#显示图像

# cv2.imshow('mask',mask0)

# #腐蚀膨胀

# erode=cv2.erode(mask,None,iterations=1)

# cv2.imshow('erode',erode)

# dilate=cv2.dilate(erode,None,iterations=1)

# cv2.imshow('dilate',dilate)

for i in range(rows):

    for j in range(cols):

        if  hsv[i,j][0] % 156 < 20:#像素点255表示白色

            image[i,j] = (255, 255, 255)  # 此处替换颜色，为BGR通道，不是RGB通道

cv2.imshow('res', image)
cv2.waitKey()