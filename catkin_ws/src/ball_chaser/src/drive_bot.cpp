#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "geometry_msgs/Twist.h"

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// This function publishes the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback is returned with the requested wheel velocities

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res)
{
  
    // Create a motor_command object of type geometry_msgs::Twist
    geometry_msgs::Twist motor_command;
  
    //ROS_INFO("DriveToTargetRequest received - w1:%1.2f, w2:%1.2f", (float)req.joint_1, (float)req.joint_2);
  
    // Set wheel velocities
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;
  
    // Publish angles to drive the robot
    motor_command_publisher.publish(motor_command);

    // Wait 3 seconds for arm to settle (take out)
    //ros::Duration(3).sleep();

    res.msg_feedback = "linear_x set : " + std::to_string(motor_command.linear.x) + " , angular_x: " + std::to_string(motor_command.angular.z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}


int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
  
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

    //  Handle ROS communication events
    ros::spin();

    return 0;
}
