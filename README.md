# image-retrieval
大型的图像识别引擎。可以有效的从几百万乃至上千万张图片中筛选出同款及相似的图片（少量图片需要修改）。
运行环境linux
依赖库opencv2.4.9
运行clear.py清除data/数据信息
运行flowDeal.py一键自动执行，图片检索结果存于data/simFile/中
flowDeal.py jpgPath
jpgPath为一级目录，里面需要包含二级目录，二级目录下图片数量不得超过10w张。
若图片数为m，并且m少于3w，请将flowDeal.py中第14行的30改为(m/1200)+1得到的具体数值.
