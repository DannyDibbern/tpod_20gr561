#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <iostream>   // std::cout

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(10.0);

	std::cout << "Hello you Robot Freaks. I'll throw you some frames." << std::endl;  

  tf::TransformBroadcaster bc;

  while(ros::ok()) {
	
    //tf::Transform transform0;
    //transform0.setOrigin( tf::Vector3(0, 0, 0) ); //Sæt X, Y, Z her. (Meter)
    //tf::Quaternion q0;
    //q0.setRPY(0, 0, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    //transform0.setRotation(q0);
    //bc.sendTransform(tf::StampedTransform(transform0, ros::Time::now(), "map", "odom"));

    tf::Transform transform1;
    transform1.setOrigin( tf::Vector3(0.51, 0, 0.06) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q1;
    q1.setRPY(0, 0, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform1.setRotation(q1);
    bc.sendTransform(tf::StampedTransform(transform1, ros::Time::now(), "rear_axle", "base_link"));

  	tf::Transform transform2;
    transform2.setOrigin( tf::Vector3(0.235, 0, 0) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q2;
    q2.setRPY(0, 0, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform2.setRotation(q2);
    bc.sendTransform(tf::StampedTransform(transform2, ros::Time::now(), "base_link", "chassis"));

    tf::Transform transform3;
    transform3.setOrigin( tf::Vector3(0.90, 0, 1.31) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q3;
    q3.setRPY(3.141593, 0.31, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform3.setRotation(q3);
    bc.sendTransform(tf::StampedTransform(transform3, ros::Time::now(), "base_link", "front_tim5xx"));

  	tf::Transform transform4;
    transform4.setOrigin( tf::Vector3(-0.51, 0, -0.28) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q4;
    q4.setRPY(0, 0, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform4.setRotation(q4);
    bc.sendTransform(tf::StampedTransform(transform4, ros::Time::now(), "base_link", "base_footprint"));

    tf::Transform transform5;
    transform5.setOrigin( tf::Vector3(1.26, 0, 0.30) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q5;
    q5.setRPY(0, 0, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform5.setRotation(q5);
    bc.sendTransform(tf::StampedTransform(transform5, ros::Time::now(), "base_link", "front_lms1xx"));

    tf::Transform transform6;
    transform6.setOrigin( tf::Vector3(-0.51, -0.40, 1.34) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q6;
    q6.setRPY(3.141593, 3.141593, 0); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform6.setRotation(q6);
    bc.sendTransform(tf::StampedTransform(transform6, ros::Time::now(), "base_link", "back_d455"));

    tf::Transform transform7;
    transform7.setOrigin( tf::Vector3(-0.94, 0, 0.35) ); //Sæt X, Y, Z her. (Meter)
    tf::Quaternion q7;
    q7.setRPY(0, 0, 3.141593); // Sæt Roll, Pitch, Yaw her.  (Radian)
    transform7.setRotation(q7);
    bc.sendTransform(tf::StampedTransform(transform7, ros::Time::now(), "base_link", "back_tim5xx"));

   r.sleep();
   }
}