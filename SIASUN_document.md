# 测试文档

标签（空格分隔）： 未分类

---
预置环境: Ubuntu 16.04/14.04(内置python2.7), ROS kinetic (文档编写时最稳定的ROS版本)
涵盖内容：


## ROS下的深度学习
### 导入训练模型
深度学习框架一般以python3.6, python3.7为编写语言，而ROS系统以c++, python2.7为源支持语言。
对于python3版本，ROS仅支持部分功能，不提供类似 坐标系转换包tf的支持(还未验证新开发的tf2程序包)。
在python2.7文件开头写上
```python
#!/usr/bin/env python
import roslib; roslib.load_manifest(PKG)
import rospy
```
在python3.6文件开头写上
```python
#!/usr/bin/env python3
import roslib; roslib.load_manifest(PKG)
import rospy
```
即可

从不同深度学习框架得到的模型文件类型不同，本文档仅说明PyTorch，并提供三种应用方式
(1)以python2.7按照原模型重新编写一次（一般复制模型结构即可），以下面代码加载模型参数到python文件中
```python
ModelName.load_state_dict(torch.load('param.pkl', map_location='gpu'))  # 如果需要用CPU运行，则改为 map_location='cpu'
```
运用python2.7，可以直接在同一文件下,同时实现ROS的扩展功能。
(2)以python3.6或3.7文件直接编写。
如果需要使用扩展功能，需要将深度学习模型的结果输出并传递到其他结点，再经由其他结点使用相应的扩展功能
(3)以c++编写模型结构，并加载模型参数
### Python测试
测试教程详见ROS官网，这里需要注意python文件需要经过cmake编译，便可以直接运行。
（注，运行前需要给予对应python文件**可执行**的权限，打开linux shell，输入'chmod file_name u+d' ）

由于ROS本身以spin循环执行代码，在python文件中，建议尽量将加载文件等部分代码，放在while not rospy.is_shutdown():之前运行，具体如下：
```
... # Prepropose the input data
... # Load model parameters
while not rospy.is_shutdown():
    result = model_name(x)
    ...
    rate.sleep()
```
### 与其他C++结点通信
ROS基本通信机制以publish和callback函数的调用完成，简单的例子见ROS官网。
通常callback函数定义在main函数之外，通过spin机制不断地调用。
所以在实际应用中，需要定义python全局变量，从而将模型结果通过callback函数传递给其他节点，具体如下：
```
trans = []
def callback(sth):
    global var_name
    var_name = sth.data # data为sth的属性，由消息定义的类型决定
    print(var_name)
    
if __name__ == '__main__':
    ...
    pub = rospy.Publisher('predicted_trajectory', trajectory_msg , queue_size=1)
    ...
    while not rospy.is_shutdown():
        ...
```  


## Debug日志
1. 'message_runtime' 需要添加在CMakeList 和 package.xml 中以便 rospy 执行时能使用 自定义的msg

2. 当使用marker做模型声明并发布tf，marker_ls接受其他tf并改变模型尺寸时，会出现如下错误
```
Invalid argument passed to canTransform argument source_frame in tf2 frame_ids cannot be empty
```
可能是因为两个文件都以相同的频率在进行模型的声明
有两种解决方案
(1) marker文件一次性定义后不再定义
(2) 去掉marker_ls，在marker中直接接受其他tf

3. roscpp 读入的消息都时vector格式的，而vector时不能通过 ojbect[index] 这种方式直接更改里面的值的。
只能像堆栈一样用'push_back'和'pop_back'

4. 根据时间进行变化，时间戳要定为ROS::Time::now()

5. 监听tf的时间戳要定为ROS::Time(0)，不然会出现如下错误
```
[ERROR] [1527985510.084914639]: Lookup would require extrapolation into the future.  Requested time 1527985510.084782991 but the latest data is at time 1527985510.079458400, when looking up transform from frame [skeleton_frame] to frame [camera_frame]
```
即，存储最新的时间戳(0)是慢于现在的时间戳now()

6. 使用Rviz的marker需要三个时间戳，listener, broadcaster 和 maker自身的时间戳, 保证三个时间戳的设定相同

7. Marker的Arrow形状本身是沿着X轴
当指定pose时， scale.x是箭头的长度; scale.y是箭身长轴长; scale.z是箭身短轴长
当指定起始点，终止点时，scale.x是箭身直径; scale.y是箭头直径; scale.z是箭头的长度(如果设为0,则会按默认箭头长度). 

8. 每个结点最多能实时发出 两个带有坐标系跟踪的marker

9. visualization_marker 必须在循环内发布 publish命令，不然不会显示

10. 要添加下面的等待tf
```
listener.waitForTransform("base_link", "tracker/right_hand", ros::Time(0), ros::Duration(1.0));
```
不然会出错
```
terminate called after throwing an instance of 'tf2::LookupException'
  what():  "base_link" passed to lookupTransform argument target_frame does not exist. 
Aborted (core dumped)
```