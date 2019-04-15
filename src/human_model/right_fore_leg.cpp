#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <tf/transform_listener.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "right_fore_leg");
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
    tf::StampedTransform knee_foot_transform;
    try{
      // Essential for lookup target frame
      listener.waitForTransform("tracker/right_knee", "tracker/right_foot", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform("tracker/right_knee", "tracker/right_foot", ros::Time(0), knee_foot_transform);

      //listener.waitForTransform("skeleton_frame", "sub_skeleton_frame", ros::Time(0), ros::Duration(1.0));
      //listener.lookupTransform("skeleton_frame", "sub_skeleton_frame", ros::Time(0), knee_foot_transform);

    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }


    visualization_msgs::Marker knee, knee_foot;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    knee.header.frame_id = knee_foot.header.frame_id = "tracker/right_knee";

    //knee.header.frame_id = knee_foot.header.frame_id = "skeleton_frame";
    
    knee.header.stamp = knee_foot.header.stamp = ros::Time(0);

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    knee.ns = knee_foot.ns = "right_fore_leg_model";
    knee.id = 0;
    knee_foot.id = 1;

    // Set the marker type.
    knee_foot.type = limb_shape;
    knee.type = joint_shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    knee.action = knee_foot.action = visualization_msgs::Marker::ADD;

 
    // Now for human model define !!!
    // Elbow
    knee.pose.orientation.w = 1.0;
    knee.scale.x = 0.1f;
    knee.scale.y = 0.1f;
    knee.scale.z = 0.1f;

    // Elbow_foot
    knee_foot.points.resize(2);
    knee_foot.points[0].x = 0.0f;
    knee_foot.points[0].y = 0.0f;
    knee_foot.points[0].z = 0.0f;
    //ROS_INFO("We got knee_foot [%f]", knee_foot_transform.getOrigin().x());
    knee_foot.points[1].x = knee_foot_transform.getOrigin().x();
    knee_foot.points[1].y = knee_foot_transform.getOrigin().y();
    knee_foot.points[1].z = knee_foot_transform.getOrigin().z();
    knee_foot.scale.x = 0.1f;
    knee_foot.scale.y = 0.1f;
    knee_foot.scale.z = 0.001f;


    // Set the color -- be sure to set alpha to something non-zero!
    knee.color.r = knee_foot.color.r = 1.0f;
    knee.color.a = knee_foot.color.a = 1.0f;

    //knee.lifetime = knee_foot.lifetime = ros::Duration();


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

    marker_pub.publish(knee);
    marker_pub.publish(knee_foot);

    r.sleep();

  }
}
