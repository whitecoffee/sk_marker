#include <ros/ros.h>
#include <tf/transform_listener.h>

#include <iostream>
#include <fstream>
using namespace std;

int main( int argc, char** argv )
{
  ros::init(argc, argv, "skeleton_ls");
  ros::NodeHandle n;
  ros::Rate r(30.0);
  
  // May be a more listener
  tf::TransformListener listener;

  int i = 0;
  while (ros::ok())
  {
    // Listen another tf
    tf::StampedTransform base_hand_transform;
    try{
      // Essential for lookup target frame
      listener.waitForTransform("base_link", "tracker/right_hand", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("base_link", "tracker/right_hand", ros::Time(0), base_hand_transform);

    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    if(i < 30)
    {
      fstream file("xue.txt", ios::app);  // iso::out overwrite, iso::app append
      file<<base_hand_transform.getOrigin().x()<<","<<base_hand_transform.getOrigin().y()<<","<<base_hand_transform.getOrigin().z()<<'\n';

      i += 1;
      ROS_INFO("[%d]", i);
    }

    r.sleep();
  }
}
