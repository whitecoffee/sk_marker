# �����ĵ�

��ǩ���ո�ָ����� δ����

---
Ԥ�û���: Ubuntu 16.04/14.04(����python2.7), ROS kinetic (�ĵ���дʱ���ȶ���ROS�汾)
�������ݣ�


## ROS�µ����ѧϰ
### ����ѵ��ģ��
���ѧϰ���һ����python3.6, python3.7Ϊ��д���ԣ���ROSϵͳ��c++, python2.7ΪԴ֧�����ԡ�
����python3�汾��ROS��֧�ֲ��ֹ��ܣ����ṩ���� ����ϵת����tf��֧��(��δ��֤�¿�����tf2�����)��
��python2.7�ļ���ͷд��
```python
#!/usr/bin/env python
import roslib; roslib.load_manifest(PKG)
import rospy
```
��python3.6�ļ���ͷд��
```python
#!/usr/bin/env python3
import roslib; roslib.load_manifest(PKG)
import rospy
```
����

�Ӳ�ͬ���ѧϰ��ܵõ���ģ���ļ����Ͳ�ͬ�����ĵ���˵��PyTorch�����ṩ����Ӧ�÷�ʽ
(1)��python2.7����ԭģ�����±�дһ�Σ�һ�㸴��ģ�ͽṹ���ɣ���������������ģ�Ͳ�����python�ļ���
```python
ModelName.load_state_dict(torch.load('param.pkl', map_location='gpu'))  # �����Ҫ��CPU���У����Ϊ map_location='cpu'
```
����python2.7������ֱ����ͬһ�ļ���,ͬʱʵ��ROS����չ���ܡ�
(2)��python3.6��3.7�ļ�ֱ�ӱ�д��
�����Ҫʹ����չ���ܣ���Ҫ�����ѧϰģ�͵Ľ����������ݵ�������㣬�پ����������ʹ����Ӧ����չ����
(3)��c++��дģ�ͽṹ��������ģ�Ͳ���
### Python����
���Խ̳����ROS������������Ҫע��python�ļ���Ҫ����cmake���룬�����ֱ�����С�
��ע������ǰ��Ҫ�����Ӧpython�ļ�**��ִ��**��Ȩ�ޣ���linux shell������'chmod file_name u+d' ��

����ROS������spinѭ��ִ�д��룬��python�ļ��У����龡���������ļ��Ȳ��ִ��룬����while not rospy.is_shutdown():֮ǰ���У��������£�
```
... # Prepropose the input data
... # Load model parameters
while not rospy.is_shutdown():
    result = model_name(x)
    ...
    rate.sleep()
```
### ������C++���ͨ��
ROS����ͨ�Ż�����publish��callback�����ĵ�����ɣ��򵥵����Ӽ�ROS������
ͨ��callback����������main����֮�⣬ͨ��spin���Ʋ��ϵص��á�
������ʵ��Ӧ���У���Ҫ����pythonȫ�ֱ������Ӷ���ģ�ͽ��ͨ��callback�������ݸ������ڵ㣬�������£�
```
trans = []
def callback(sth):
    global var_name
    var_name = sth.data # dataΪsth�����ԣ�����Ϣ��������;���
    print(var_name)
    
if __name__ == '__main__':
    ...
    pub = rospy.Publisher('predicted_trajectory', trajectory_msg , queue_size=1)
    ...
    while not rospy.is_shutdown():
        ...
```  


## Debug��־
1. 'message_runtime' ��Ҫ�����CMakeList �� package.xml ���Ա� rospy ִ��ʱ��ʹ�� �Զ����msg

2. ��ʹ��marker��ģ������������tf��marker_ls��������tf���ı�ģ�ͳߴ�ʱ����������´���
```
Invalid argument passed to canTransform argument source_frame in tf2 frame_ids cannot be empty
```
��������Ϊ�����ļ�������ͬ��Ƶ���ڽ���ģ�͵�����
�����ֽ������
(1) marker�ļ�һ���Զ�����ٶ���
(2) ȥ��marker_ls����marker��ֱ�ӽ�������tf

3. roscpp �������Ϣ��ʱvector��ʽ�ģ���vectorʱ����ͨ�� ojbect[index] ���ַ�ʽֱ�Ӹ��������ֵ�ġ�
ֻ�����ջһ����'push_back'��'pop_back'

4. ����ʱ����б仯��ʱ���Ҫ��ΪROS::Time::now()

5. ����tf��ʱ���Ҫ��ΪROS::Time(0)����Ȼ��������´���
```
[ERROR] [1527985510.084914639]: Lookup would require extrapolation into the future.  Requested time 1527985510.084782991 but the latest data is at time 1527985510.079458400, when looking up transform from frame [skeleton_frame] to frame [camera_frame]
```
�����洢���µ�ʱ���(0)���������ڵ�ʱ���now()

6. ʹ��Rviz��marker��Ҫ����ʱ�����listener, broadcaster �� maker�����ʱ���, ��֤����ʱ������趨��ͬ

7. Marker��Arrow��״����������X��
��ָ��poseʱ�� scale.x�Ǽ�ͷ�ĳ���; scale.y�Ǽ����᳤; scale.z�Ǽ�����᳤
��ָ����ʼ�㣬��ֹ��ʱ��scale.x�Ǽ���ֱ��; scale.y�Ǽ�ͷֱ��; scale.z�Ǽ�ͷ�ĳ���(�����Ϊ0,��ᰴĬ�ϼ�ͷ����). 

8. ÿ����������ʵʱ���� ������������ϵ���ٵ�marker

9. visualization_marker ������ѭ���ڷ��� publish�����Ȼ������ʾ

10. Ҫ�������ĵȴ�tf
```
listener.waitForTransform("base_link", "tracker/right_hand", ros::Time(0), ros::Duration(1.0));
```
��Ȼ�����
```
terminate called after throwing an instance of 'tf2::LookupException'
  what():  "base_link" passed to lookupTransform argument target_frame does not exist. 
Aborted (core dumped)
```