#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "right_fore_arm");
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
    tf::StampedTransform elbow_hand_transform;
    try{
      // Essential for lookup target frame
      listener.waitForTransform("tracker/right_elbow", "tracker/right_hand", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/right_elbow", "tracker/right_hand", ros::Time(0), elbow_hand_transform);

      //listener.waitForTransform("skeleton_frame", "sub_skeleton_frame", ros::Time(0), ros::Duration(1.0));
      //listener.lookupTransform("skeleton_frame", "sub_skeleton_frame", ros::Time(0), elbow_hand_transform);

    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }


    visualization_msgs::Marker elbow, elbow_hand;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    elbow.header.frame_id = elbow_hand.header.frame_id = "tracker/right_elbow";

    //elbow.header.frame_id = elbow_hand.header.frame_id = "skeleton_frame";
    
    elbow.header.stamp = elbow_hand.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    elbow.ns = elbow_hand.ns = "right_fore_arm_model";
    elbow.id = 0;
    elbow_hand.id = 1;

    // Set the marker type.
    elbow_hand.type = limb_shape;
    elbow.type = joint_shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    elbow.action = elbow_hand.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // Elbow
    elbow.pose.orientation.w = 1.0;
    elbow.scale.x = 0.1f;
    elbow.scale.y = 0.1f;
    elbow.scale.z = 0.1f;

    // Elbow_hand
    elbow_hand.points.resize(2);
    elbow_hand.points[0].x = 0.0f;
    elbow_hand.points[0].y = 0.0f;
    elbow_hand.points[0].z = 0.0f;
    //ROS_INFO("We got elbow_hand [%f]", elbow_hand_transform.getOrigin().x());
    elbow_hand.points[1].x = elbow_hand_transform.getOrigin().x();
    elbow_hand.points[1].y = elbow_hand_transform.getOrigin().y();
    elbow_hand.points[1].z = elbow_hand_transform.getOrigin().z();
    elbow_hand.scale.x = 0.1f;
    elbow_hand.scale.y = 0.1f;
    elbow_hand.scale.z = 0.001f;


    // Set the color -- be sure to set alpha to something non-zero!
    elbow.color.r = elbow_hand.color.r = 1.0f;
    elbow.color.a = elbow_hand.color.a = 1.0f;

    //elbow.lifetime = elbow_hand.lifetime = ros::Duration();


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

    marker_pub.publish(elbow);
    marker_pub.publish(elbow_hand);

    r.sleep();

  }
}
