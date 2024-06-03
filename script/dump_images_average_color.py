import json
import cv2
import os
import numpy as np

inpath = input("Enter the path of the image: ")
outpath = input("Enter the path to save the output: ")

def getAllFiles(path):
    files = []
    for r, d, f in os.walk(path):
        for file in f:
            if '.jpg' in file or '.png' in file:
                files.append(os.path.join(r, file))
    return files


def getImages(files):
    images = []
    for file in files:
        img = cv2.imread(file)
        images.append(img)
    return images


def getImageAveragedColor(image):
    # 转换颜色空间为RGB
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    # 计算颜色通道的平均值
    average_color = np.mean(image_rgb, axis=(0, 1))
    # 转换为十六进制字符串
    average_color_hex = '{:02x}{:02x}{:02x}'.format(
        int(average_color[0]), int(average_color[1]), int(average_color[2])
    )
    return average_color_hex


def main():
    files = getAllFiles(inpath)
    images = getImages(files)
    dic = {}
    for i in range(len(images)):
        img = images[i]
        average_color = getImageAveragedColor(img)
        filename = files[i]
        filename = filename[filename.rfind('\\')+1:]
        dic[filename] = average_color
    json.dump(dic,open(outpath, 'w'), indent=2)


if __name__ == '__main__':
    main()