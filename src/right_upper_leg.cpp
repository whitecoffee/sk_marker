#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "right_upper_leg");
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
    tf::StampedTransform hip_knee_transform;
    try{
      // Essential for lookup target frame
      listener.waitForTransform("tracker/right_hip", "tracker/right_knee", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/right_hip", "tracker/right_knee", ros::Time(0), hip_knee_transform);

      //listener.waitForTransform("camera_frame", "skeleton_frame", ros::Time(0), ros::Duration(1.0));
      //listener.lookupTransform("camera_frame", "skeleton_frame", ros::Time(0), hip_knee_transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }


    visualization_msgs::Marker hip, hip_knee;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    hip.header.frame_id = hip_knee.header.frame_id = "tracker/right_hip";

    //hip.header.frame_id = hip_knee.header.frame_id = "camera_frame";
    
    hip.header.stamp = hip_knee.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    hip.ns = hip_knee.ns = "right_upper_leg_model";
    hip.id = 0;
    hip_knee.id = 1;

    // Set the marker type.
    hip_knee.type = limb_shape;
    hip.type = joint_shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    hip.action = hip_knee.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // hip 
    hip.pose.orientation.w = 1.0;
    // Set the scale of the marker -- 1x1x1 here means 1m on a sider
    hip.scale.x = 0.1f;
    hip.scale.y = 0.1f;
    hip.scale.z = 0.1f;

    // hip_knee
    // Index 0 is start point
    hip_knee.points.resize(2);
    hip_knee.points[0].x = 0.0f;
    hip_knee.points[0].y = 0.0f;
    hip_knee.points[0].z = 0.0f;
    //ROS_INFO("We got hip_knee [%f]", hip_knee_transform.getOrigin().x());
    hip_knee.points[1].x = hip_knee_transform.getOrigin().x();
    hip_knee.points[1].y = hip_knee_transform.getOrigin().y();
    hip_knee.points[1].z = hip_knee_transform.getOrigin().z();
    hip_knee.scale.x = 0.1f;
    hip_knee.scale.y = 0.1f;
    hip_knee.scale.z = 0.001f;
    //marker.scale.z = sqrt(pow(ls_transform.getOrigin().x(), 2) + pow(ls_transform.getOrigin().y(), 2) + pow(ls_transform.getOrigin().z(), 2));


    // Set the color -- be sure to set alpha to something non-zero!
    hip.color.r = hip_knee.color.r = 1.0f;
    hip.color.a = hip_knee.color.a = 1.0f;

    hip.lifetime = hip_knee.lifetime = ros::Duration();


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
    marker_pub.publish(hip);
    marker_pub.publish(hip_knee);

    r.sleep();

  }
}
