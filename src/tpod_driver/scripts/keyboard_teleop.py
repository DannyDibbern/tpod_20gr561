#!/usr/bin/env python
import roslib
import rospy
#from aauship_control.msg import *
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64

import sys, select, termios, tty

def getKey():
	tty.setraw(sys.stdin.fileno())
	select.select([sys.stdin], [], [], 0)
	key = sys.stdin.read(1)
	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
	return key

vel_now = 0
vel_left = 0
vel_right = 0
count = 0
state = 0

if __name__=="__main__":
	settings = termios.tcgetattr(sys.stdin)
	pubsteer = rospy.Publisher("set_steering_angle", Float64, queue_size=1000)
	pubvel = rospy.Publisher("set_accelerator_position", Float64, queue_size=1000)
	pubbrake = rospy.Publisher("set_brake_position", Float64, queue_size=1000)
	rospy.init_node('keyboard_teleop_node')

	SAT_VEL = 2
	SAT_TURN = 22000
	turn = 0
	vel = 0
	brake = 2.4
	#SAT_DOWN = -200
	#DEBOUNCE_COUNT = 1
	STOP_MODE = 0
	ON_MODE = 1
	pub_msg = Float64()

	while(1):
		key = getKey()
		print(key)

		if(state == ON_MODE):
		# Step operation: Cross-up to increase speed by 10 and cross-down to decrease it
			if (key == 'w'):
				vel += 0.1
			if (key == 's'):
				vel -= 0.1
	
			# To turn keep pressing cross left/right button.
			if (key == 'a'):
				turn -= -1000
			if (key == 'd'):
				turn += -1000
			
			if (key == 'q'):
				brake -= 0.10
			if (key == 'e'):
				brake += 0.10

			# Check for saturation
			if (vel > SAT_VEL):
		 		vel = SAT_VEL
			if (vel < 0):
				vel = 0
				
			if (turn > SAT_TURN):
		 		turn = SAT_TURN
			if (turn < -SAT_TURN):
				turn = -SAT_TURN

			if (brake > 3.6):
				brake = 3.6
			if (brake < 1.2):
				brake = 1.2

		else: #if(state == ON_MODE):
		# To start the boat, press the triangle
			if (key == '1'):
				state=ON_MODE
				vel_now=0

		# Emergency Stop: Press the circle
		if (key == '0'):
			vel=0
			turn=0
			brake=2.4
			state=STOP_MODE
		
		pub_msg.data = turn
		pubsteer.publish(pub_msg)
		print("Steer msg: " + str(pub_msg))

		pub_msg.data = vel
		pubvel.publish(pub_msg)
		print("Vel msg: " + str(pub_msg))

		pub_msg.data = brake
		pubbrake.publish(pub_msg)
		print("Brake msg: " + str(pub_msg))
		
		if (key == '\x03'):
			break