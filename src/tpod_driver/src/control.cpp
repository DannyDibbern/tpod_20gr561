#include "ros/ros.h"
#include "std_msgs/String.h"
#include "serial/serial.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

float left, right = 0;
float left_old, right_old = 0;
float delta_left, delta_right = 0;
float yaw = 0;
float x, y, delta_x, delta_y = 0;

float xi = 0;
float psi = 0;
float psi_total = 0;

float yaw_goal = 0.0;
float x_goal = 0;
float y_goal = 0;

float set_steering_angle = 0;
float set_accelerator_position = 0;
float set_brake_position = 2.5;

float dif_x = 0;
float dif_y = 0;

//float cmd_speed;
float omega = 0;
float v = 0;

void tpod_goal_sub_callback(const geometry_msgs::Pose2D::ConstPtr& new_msg)
	{	
	//x_goal = new_msg->x;
	//y_goal = new_msg->y;
	//yaw_goal = new_msg->theta;
	}

void tpod_command_sub_callback(const geometry_msgs::Twist::ConstPtr& new_msg) {
	v = new_msg->linear.x;
	omega = new_msg->angular.z;
	set_accelerator_position = v;
	set_steering_angle = atan(1.64/(v/omega)) * 25478;
	}

void ticks_left_sub_callback(const std_msgs::Int64::ConstPtr& new_msg)
{
	left = new_msg->data;
}

void ticks_right_sub_callback(const std_msgs::Int64::ConstPtr& new_msg)
{
  	right = new_msg->data;
 
	// === Do the math ===	
	left = left * 0.00459078; // convert from tick to meter
	right = right * 0.00353982; //The odd numbers comes from calibration
    
 	delta_left = (left - left_old); // Difference in movement since last iteration
	delta_right = (right - right_old); 

	left_old = left; // save value from this iteration
 	right_old = right;

  	yaw = (right - left)/0.98; // Calculate yaw (0.98 = wheelbase)

	delta_x = ((delta_left + delta_right)/2) * cos(yaw); // calculate difference in x movement
	delta_y = ((delta_left + delta_right)/2) * sin(yaw); // calculate difference in y movement
	
	x = x + delta_x; // calculate accumulated x movement
	y = y + delta_y; // calculate accumulated y movement

	//std::cout<<left<<" "<<right<<" "<<yaw<<" "<<x<<" "<<y<<std::endl;
	
	dif_x = x_goal - x;
	dif_y = y_goal - y;

	xi = atan(dif_y/dif_x)-yaw;	
	psi = -atan(dif_y/dif_x);
	psi_total = psi + yaw_goal;
	//set_steering_angle = ((4.6 * xi) - (2.8 * psi_total)) * 25478;

	if (set_steering_angle > 20000) set_steering_angle = 20000;
	if (set_steering_angle <-20000) set_steering_angle = -20000;
 	
	float distance = (sqrt(dif_x*dif_x + dif_y*dif_y));

	if (distance > 1) set_accelerator_position = 0.6;
	if (distance < 1 && distance > 0) set_accelerator_position = 0.4;
	if (distance < 0) set_accelerator_position = 0;
    
	std::cout<<"Speed: "<< set_accelerator_position <<" Steer: "<< set_steering_angle <<std::endl;		

}

class TpodControl
{
public:
	TpodControl(ros::NodeHandle& ros_nh){

	tpod_pose_pub = ros_nh.advertise<geometry_msgs::Pose2D>("tpod_pose", 1000);
	set_steering_angle_pub = ros_nh.advertise<std_msgs::Float64>("set_steering_angle", 1000);
	set_accelerator_position_pub = ros_nh.advertise<std_msgs::Float64>("set_accelerator_position", 1000);
  	set_brake_position_pub = ros_nh.advertise<std_msgs::Float64>("set_brake_position", 1000);
	}

	void timer_func(const ros::TimerEvent& te) {
		
    geometry_msgs::Pose2D pose;
	pose.x = x;
	pose.y = y;
	pose.theta = yaw;
	tpod_pose_pub.publish(pose);

	std_msgs::Float64 ssa;
    ssa.data = set_steering_angle;
    set_steering_angle_pub.publish(ssa);

    std_msgs::Float64 sap;
    sap.data = set_accelerator_position;
    set_accelerator_position_pub.publish(sap);

	std_msgs::Float64 sbp;
    sbp.data = 2.5;
    set_brake_position_pub.publish(sbp);

	}

private: 
	ros::Publisher tpod_pose_pub;
	ros::Publisher set_steering_angle_pub;
	ros::Publisher set_accelerator_position_pub;
    ros::Publisher set_brake_position_pub;
};

int main(int argc, char **argv) 
{
  ros::init(argc, argv, "control");
	
	ros::NodeHandle ros_nh;

 	ros::Subscriber ticks_left_sub = ros_nh.subscribe("ticks_left", 1000, ticks_left_sub_callback);
  	ros::Subscriber ticks_right_sub = ros_nh.subscribe("ticks_right", 1000, ticks_right_sub_callback);
	ros::Subscriber tpod_goal_sub = ros_nh.subscribe("tpod_goal", 1000, tpod_goal_sub_callback);
	ros::Subscriber tpod_command = ros_nh.subscribe("cmd_vel", 1000, tpod_command_sub_callback);	

	TpodControl tc(ros_nh);

	ros::Timer _send_setpoints_timer = ros_nh.createTimer(ros::Duration(0.1), &TpodControl::timer_func, &tc);
	ROS_INFO("Controller is running");
  
	ros::spin();
	
  return 0;
}

