#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>
int main( int argc, char** argv )
{
  ros::init(argc, argv, "head");
  ros::NodeHandle n;
  ros::Rate r(30.0);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Set our initial shape types
  uint32_t joint_shape = visualization_msgs::Marker::SPHERE;
  // May be a more listener
  tf::TransformListener listener;

  while (ros::ok())
  {
    // Listen another tf
    tf::StampedTransform neck_head_transform;
    try{
      // Essential for lookup target frame
      listener.waitForTransform("tracker/neck", "tracker/head", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/neck", "tracker/head", ros::Time(0), neck_head_transform);

    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    visualization_msgs::Marker head, neck;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    head.header.frame_id = "tracker/head";
    neck.header.frame_id = "tracker/neck";

    //hand.header.frame_id = "sub_skeleton_frame";
    
    head.header.stamp = neck.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    head.ns = neck.ns = "head_model";
    head.id = 0;
    neck.id = 1;

    // Set the marker type.
    head.type = joint_shape;
    neck.type = visualization_msgs::Marker::ARROW;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    head.action = neck.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // Hands
    head.pose.orientation.w = 1.0;
    head.scale.x = 0.2f;
    head.scale.y = 0.2f;
    head.scale.z = 0.2f;

    // Neck
    neck.points.resize(2);
    neck.points[0].x = 0.0f;
    neck.points[0].y = 0.0f;
    neck.points[0].z = 0.0f;
    neck.points[1].x = neck_head_transform.getOrigin().x();
    neck.points[1].y = neck_head_transform.getOrigin().y();
    neck.points[1].z = neck_head_transform.getOrigin().z();
    neck.scale.x = 0.1f;
    neck.scale.y = 0.1f;
    neck.scale.z = 0.001f;


    // Set the color -- be sure to set alpha to something non-zero!
    head.color.r = neck.color.r = 1.0f;
    head.color.a = neck.color.a = 1.0f;


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

    marker_pub.publish(head);
    marker_pub.publish(neck);

    r.sleep();

  }
}
