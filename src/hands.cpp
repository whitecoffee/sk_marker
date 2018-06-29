#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "hands");
  ros::NodeHandle n;
  ros::Rate r(30.0);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Set our initial shape types
  uint32_t joint_shape = visualization_msgs::Marker::SPHERE;

  while (ros::ok())
  {
    visualization_msgs::Marker right_hand, left_hand;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    right_hand.header.frame_id = "tracker/right_hand";
    left_hand.header.frame_id = "tracker/left_hand";

    //hand.header.frame_id = "sub_skeleton_frame";
    
    right_hand.header.stamp = left_hand.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    right_hand.ns = left_hand.ns = "hands_model";
    right_hand.id = 0;
    left_hand.id = 1;

    // Set the marker type.
    right_hand.type = left_hand.type = joint_shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    right_hand.action = left_hand.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // Hands
    right_hand.pose.orientation.w = left_hand.pose.orientation.w = 1.0;
    right_hand.scale.x = left_hand.scale.x = 0.1f;
    right_hand.scale.y = left_hand.scale.y = 0.1f;
    right_hand.scale.z = left_hand.scale.z = 0.1f;


    // Set the color -- be sure to set alpha to something non-zero!
    right_hand.color.r = left_hand.color.r = 1.0f;
    right_hand.color.a = left_hand.color.a = 1.0f;

    //elbow.lifetime = elbow_hand.lifetime = hand.lifetime = ros::Duration();


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

    marker_pub.publish(right_hand);
    marker_pub.publish(left_hand);

    r.sleep();

  }
}
