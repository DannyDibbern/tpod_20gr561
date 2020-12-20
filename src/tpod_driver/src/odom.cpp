#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_broadcaster.h>
#include "serial/serial.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int64.h>
#include <nav_msgs/Odometry.h>

double left, right, delta_left, delta_right, left_old, right_old = 0;
double yaw, yaw_now, yaw_old = 0;
double x, y, delta_x, delta_y = 0;
double vel_left, vel_right, vel_yaw, vel_x = 0;

double time_now, time_old;
ros::Time timenow, timeold;

nav_msgs::Odometry odom;

void ticks_left_sub_callback(const std_msgs::Int64::ConstPtr& new_msg)
{
	left = new_msg->data;
    left = round(left * 1.02);
}

void ticks_right_sub_callback(const std_msgs::Int64::ConstPtr& new_msg)
{
    right = new_msg->data;

 	timenow = ros::Time::now();
    time_now = ros::Time::now().toSec();

//  === Do the math ===	
	left = left * 0.0034625; // Convert from tick to meter
    right = right * 0.0034625; //0.0034625; 

    delta_left = (left - left_old); // Difference in movement since last iteration
    delta_right = (right - right_old);

    yaw = (right - left)/0.98; // Calculate yaw (0.98 = wheelbase)

	delta_x = ((delta_left + delta_right)/2) * cos(yaw); // Calculate difference in x movement
	delta_y = ((delta_left + delta_right)/2) * sin(yaw); // Calculate difference in y movement
	
    x += delta_x; // Calculate accumulated x,y,yaw movement
    y += delta_y;
    yaw_now += yaw;

    // Find velocities

    vel_left = (left - left_old) * (time_now - time_old);
    vel_right = (right - right_old) * (time_now - time_old);
    vel_yaw = (yaw_now - yaw_old) * (time_now - time_old);
    vel_x = (vel_left + vel_right)/2;

    std::cout<<"%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
    std::cout<<"X: "<<x<<" Y: "<<y<<std::endl;
    std::cout<<"%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
    std::cout<<"Left distance (m): "<<left<<" Right distance (m): "<<right<< " Yaw: " <<yaw<<" Yaw now: "<<yaw_now<<std::endl;
    std::cout<<"%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
    std::cout << "Old left: " << left_old << " Old right: " << right_old << " Old yaw: " << yaw_old << " Now time: " << time_now << " Old time: " << time_old << std::endl;
    std::cout<<"%%%%%%%%%%%%%%%%%%%%%\n"<<std::endl;

    yaw_old = yaw_now;  // Save value from this iteration    
    left_old = left;
    right_old = right;
    timeold = timenow;
    time_old = time_now;

    //std::cout<<"\n%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
    //std::cout << "Vel left: " << vel_left << " Vel right: " << vel_right << " Vel yaw: " << vel_yaw << " Vel x: " << vel_x << std::endl;
 
//  === Broadcast transformation ===
  	//static tf::TransformBroadcaster br;
    //tf::Transform transform;
    //transform.setOrigin( tf::Vector3(x, y, 0.22) ); //Sæt X og Y her. (Ingen bevægelse i Z-retning.)
    //tf::Quaternion q;
    //q.setRPY(0, 0, yaw); //Sæt Roll, Pitch, Yaw her. (Kun Yaw er aktuelt.) (Radian)
  
	//transform.setRotation(q);

	//br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "rear_axle"));

//  === Broadcast transformation ===
    static tf::TransformBroadcaster br;

    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(yaw); // Create quaternion from yaw

    geometry_msgs::TransformStamped trans;
    trans.header.stamp = timenow;
    trans.header.frame_id = "odom";
    trans.child_frame_id = "rear_axle";

    trans.transform.translation.x = x;
    trans.transform.translation.y = y;
    trans.transform.translation.z = 0.22;
    trans.transform.rotation = odom_quat;

    // Send the transform
    br.sendTransform(trans);

//  === Broadcast odometry message ===
    odom.header.stamp = timenow;
    odom.header.frame_id = "odom";

    //Set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0;
    odom.pose.pose.orientation = odom_quat;

    //Set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vel_x;
    odom.twist.twist.linear.y = 0;
    odom.twist.twist.angular.z = vel_yaw;
}

int main(int argc, char **argv) 
{
    ros::init(argc, argv, "odom");

    ros::NodeHandle ros_nh;

//	ros::Time timeold = ros::Time::now();
//	double time_old = ros::Time::now().toSec();

    ros::Publisher odom_pub = ros_nh.advertise<nav_msgs::Odometry>("odom_wheel", 50);
    ros::Subscriber ticks_left_sub = ros_nh.subscribe("ticks_left", 1000, ticks_left_sub_callback);
    ros::Subscriber ticks_right_sub = ros_nh.subscribe("ticks_right", 1000, ticks_right_sub_callback);
    
    ros::Rate r(1.0);
    while(ros_nh.ok()){
        ros::spinOnce();

        odom_pub.publish(odom);

        r.sleep();
    }
  return 0;
}