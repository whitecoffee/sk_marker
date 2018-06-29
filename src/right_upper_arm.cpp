#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "right_upper_arm");
  ros::NodeHandle n;
  ros::Rate r(30.0);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Set our initial shape types
  uint32_t limb_shape = visualization_msgs::Marker::ARROW;
  uint32_t joint_shape = visualization_msgs::Marker::SPHERE;
  
  // May be a more listener
  tf::TransformListener listener;

  while (ros::ok())
  {
    // Listen another tf
    tf::StampedTransform shoulder_elbow_transform;
    try{
      // Essential for lookup target frame
      listener.waitForTransform("tracker/right_shoulder", "tracker/right_elbow", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/right_shoulder", "tracker/right_elbow", ros::Time(0), shoulder_elbow_transform);

      //listener.waitForTransform("camera_frame", "skeleton_frame", ros::Time(0), ros::Duration(1.0));
      //listener.lookupTransform("camera_frame", "skeleton_frame", ros::Time(0), shoulder_elbow_transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }


    visualization_msgs::Marker shoulder, shoulder_elbow;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    shoulder.header.frame_id = shoulder_elbow.header.frame_id = "tracker/right_shoulder";

    //shoulder.header.frame_id = shoulder_elbow.header.frame_id = "camera_frame";
    
    shoulder.header.stamp = shoulder_elbow.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    shoulder.ns = shoulder_elbow.ns = "right_upper_arm_model";
    shoulder.id = 0;
    shoulder_elbow.id = 1;

    // Set the marker type.
    shoulder_elbow.type = limb_shape;
    shoulder.type = joint_shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    shoulder.action = shoulder_elbow.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // Shoulder 
    shoulder.pose.orientation.w = 1.0;
    // Set the scale of the marker -- 1x1x1 here means 1m on a sider
    shoulder.scale.x = 0.1f;
    shoulder.scale.y = 0.1f;
    shoulder.scale.z = 0.1f;

    // Shoulder_elbow
    // Index 0 is start point
    shoulder_elbow.points.resize(2);
    shoulder_elbow.points[0].x = 0.0f;
    shoulder_elbow.points[0].y = 0.0f;
    shoulder_elbow.points[0].z = 0.0f;
    ROS_INFO("We got shoulder_elbow [%f]", shoulder_elbow_transform.getOrigin().x());
    shoulder_elbow.points[1].x = shoulder_elbow_transform.getOrigin().x();
    shoulder_elbow.points[1].y = shoulder_elbow_transform.getOrigin().y();
    shoulder_elbow.points[1].z = shoulder_elbow_transform.getOrigin().z();
    shoulder_elbow.scale.x = 0.1f;
    shoulder_elbow.scale.y = 0.1f;
    shoulder_elbow.scale.z = 0.001f;
    //marker.scale.z = sqrt(pow(ls_transform.getOrigin().x(), 2) + pow(ls_transform.getOrigin().y(), 2) + pow(ls_transform.getOrigin().z(), 2));


    // Set the color -- be sure to set alpha to something non-zero!
    shoulder.color.r = shoulder_elbow.color.r = 1.0f;
    shoulder.color.a = shoulder_elbow.color.a = 1.0f;

    shoulder.lifetime = shoulder_elbow.lifetime = ros::Duration();


    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the markers");
      sleep(1);
    }
    marker_pub.publish(shoulder);
    marker_pub.publish(shoulder_elbow);

    r.sleep();

  }
}
