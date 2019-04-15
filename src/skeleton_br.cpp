// Broadcast 'trasform' between the base link of robot "base_link" and base link of human model "world"

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "skeleton_br");
  ros::NodeHandle node;

  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(30.0);
  while (node.ok())
  {                                                                

    transform.setOrigin(tf::Vector3( 0.0, 0.0, 0.0 ));
    transform.setRotation( tf::Quaternion(0, 0, 0.3827, 0.9239) );  // Rotation of 45 degree
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "world"));

    rate.sleep();
  }
  return 0;
};
	
