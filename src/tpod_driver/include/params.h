#ifndef PARAMS_H
#define PARAMS_H

#include <iostream>
#include <cstring>
#include <cmath>
#include <sstream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int64.h"
#include "math.h"
#include <string>
#include <complex>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>
#include <fstream>
#include <vector>
#include "sensor_msgs/Imu.h"
#include "tf/transform_broadcaster.h"
#include "tf/transform_datatypes.h"
//#include <tf2_ros/static_transform_broadcaster.h>
//#include <geometry_msgs/TransformStamped.h>
//#include <cstdio>
//#include <tf2/LinearMath/Quaternion.h>
//#include <tf2_ros/transform_broadcaster.h>

#define PI 3.1415926535897
//ROS STUFF
#define TS 0.08 
//Cart parameters
#define WTR 0.98 //rearwheeltrack
#define MAXSTEERL 20000
#define MAXSTEERR -20000
//unit convertions
#define STEERPOS2ANGL 31812 
#define TICKL2METER 0.00374068//0.0038//0.00339271 
#define TICKR2METER 0.00331225//0.0044////00.0037535
#define PIPERCENT 0.318
#define THROTTLE2METER 0.769823
#define THROTTLECONST 0.260899
//Controller
#define GPSTEERING 1.41421//2.23607 //1.41421//G_P ->steering
#define GDSTEERINGCON 1.339 //G_D ->steering
#define KP 1.5
#define KI 1.05
#define KD 0.55
//PATH
#define THRESHOLDPOS 0.61
#define THRESY 0.1

//#define OLD_AND_WORKS 

#endif /*PARAMS_H*/
