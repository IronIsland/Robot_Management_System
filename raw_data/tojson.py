import json
# 打开文本文件
fp = open("output(2).txt", 'r')
# 使用readlines读取
lines = fp.readlines()

mylist=[]
dict = {}

for line in lines:
    # 将读取的每行内容过滤掉换行符,如果不加这个条件，输入的内容中将会添加换行符\n
    line = line.strip('\n')
    ss = line.split(',')  # 将每行内容根据:分割
    mylist.append(ss)
    #print(ss)


mylist=[i for i in mylist for j in i if j !='']
print(mylist)

temp=0
xlist=[]
ylist=[]
zlist=[]
axis=[]
for i in mylist:
    x = int(i[0])
    y = int(i[1])
    z = int(i[2])
    print(x,y,z)
    xlist.append(x)
    ylist.append(y)
    zlist.append(z)
    temp+=1
    axis.append(str(temp))

dict

#重新写入json文件，并且以规定形式保存
with open('1.json',mode="w",encoding='utf-8') as f:
    json.dump({"x_dis":xlist,"y_dis":ylist,"z_dis":zlist,"dis_axis":axis},f)




# data = list
# data = json.dumps(data)
# print(data)