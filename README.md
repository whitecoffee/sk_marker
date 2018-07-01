# sk_marker
## How to run this package?
Enter the following code in linux terminal.

(1) CATKIN_MAKE this rospackage

```
>> cd you_worksapce/
>> catkin_make

```
(2) Run the Kinect tracker code 

(3) roslaunch our launch file
```
>> roslaunch sk_maker sk_demo.launch
```

Then you will get the similar image as following in your rviz environment
![The result you should see](https://raw.githubusercontent.com/whitecoffee/sk_marker/master/image/model.png)

## Costumize human model
We build this model with simplest clinder like shape. 
Of course, you can try your own color and own shape you want in this package.

And here are some advice from us:

(1) If you want to change shape with other type of rviz maker, PLEASE don't do it! 

Since this model is built with tf listener, which akin to Kinect tracker frame. 
Other type of rviz marker won't connect each ohter like our model(and flexible as our model) 

(2)If you want to change color of this model, search following code in each file of 'src' package,
```
shoulder.color.r = ... 
shoulder.color.a = ...
```
and change as your wish. The method 'color' has four abbrs -- (r,g,b,a) from 1(dark) to 0(light).

'a' is transparency, be careful to set it.

(3)If you want to change shape with your designed, try '<<a>mesh</a>>' as tutorial and change the following code
```
uint32_t limb_shape = visualization_msgs::Marker::ARROW;
```

## Something you may wanna know

It seems that divde human model to several files is not wisdom. 
But this way will give more effectiveness of connnection than one node take overweight burden of building model.
And most importantly, this kind of expansion is better in real-time running.
