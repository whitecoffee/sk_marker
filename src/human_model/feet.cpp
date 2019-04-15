#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "feet");
  ros::NodeHandle n;
  ros::Rate r(30.0);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Set our initial shape types
  uint32_t joint_shape = visualization_msgs::Marker::SPHERE;

  while (ros::ok())
  {
    visualization_msgs::Marker right_foot, left_foot;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    right_foot.header.frame_id = "tracker/right_foot";
    left_foot.header.frame_id = "tracker/left_foot";

    //foot.header.frame_id = "sub_skeleton_frame";
    
    right_foot.header.stamp = left_foot.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    right_foot.ns = left_foot.ns = "feet_model";
    right_foot.id = 0;
    left_foot.id = 1;

    // Set the marker type.
    right_foot.type = left_foot.type = joint_shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    right_foot.action = left_foot.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // Hands
    right_foot.pose.orientation.w = left_foot.pose.orientation.w = 1.0;
    right_foot.scale.x = left_foot.scale.x = 0.1f;
    right_foot.scale.y = left_foot.scale.y = 0.1f;
    right_foot.scale.z = left_foot.scale.z = 0.1f;


    // Set the color -- be sure to set alpha to something non-zero!
    right_foot.color.r = left_foot.color.r = 1.0f;
    right_foot.color.a = left_foot.color.a = 1.0f;

    //elbow.lifetime = elbow_foot.lifetime = foot.lifetime = ros::Duration();


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

    marker_pub.publish(right_foot);
    marker_pub.publish(left_foot);

    r.sleep();

  }
}
