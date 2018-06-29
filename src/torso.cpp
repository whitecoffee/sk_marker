#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "torso");
  ros::NodeHandle n;
  ros::Rate r(30.0);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  tf::TransformListener listener;

  while (ros::ok())
  {
    // Listen another tf
    tf::StampedTransform torso_neck;
    tf::StampedTransform shoulder_length;
    try{
      // Essential for lookup target frame

      //listener.waitForTransform("camera_frame", "skeleton_frame", ros::Time(0), ros::Duration(1.0));
      //listener.lookupTransform("camera_frame", "skeleton_frame", ros::Time(0), transform);

      listener.waitForTransform("tracker/torso", "tracker/neck", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/torso", "tracker/neck", ros::Time(0), torso_neck);

      listener.waitForTransform("tracker/right_shoulder", "tracker/left_shoulder", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/right_shoulder", "tracker/left_shoulder", ros::Time(0), shoulder_length);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    visualization_msgs::Marker torso;

    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    //torso.header.frame_id = "torso";
    torso.header.frame_id = "tracker/torso";
    
    torso.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    torso.ns = "torso_model";
    torso.id = 0;

    // Set the marker type.
    torso.type = visualization_msgs::Marker::ARROW;
    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    torso.action = visualization_msgs::Marker::ADD;

    torso.points.resize(2);
    torso.points[0].x = -torso_neck.getOrigin().x();
    torso.points[0].y = -torso_neck.getOrigin().y();
    torso.points[0].z = -torso_neck.getOrigin().z();
    //ROS_INFO("We got torso [%f]", torso_transform.getOrigin().x());
    torso.points[1].x = torso_neck.getOrigin().x();
    torso.points[1].y = torso_neck.getOrigin().y();
    torso.points[1].z = torso_neck.getOrigin().z();
    torso.scale.x = torso.scale.y = sqrt(pow(shoulder_length.getOrigin().x(), 2) + pow(shoulder_length.getOrigin().y(), 2) + pow(shoulder_length.getOrigin().z(), 2));
    torso.scale.z = 0.001f;

    // Line strip is red
    torso.color.r = 1.0f;
    torso.color.a = 1.0f;

    //torso.lifetime = ros::Duration();


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

    marker_pub.publish(torso);


    r.sleep();

  }
}
