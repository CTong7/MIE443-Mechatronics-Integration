/* TO DO:

- Create header files for functions
- Move function source code into implementation files
- Write a Random Walk

*/

/* How to Launch */
/*

Gazebo:
roslaunch mie443_contest1 turtlebot_world.launch world:=1

Teleop:
roslaunch turtlebot_teleop keyboard_teleop.launch

Source Code:
cd ~/catkin_ws
catkin_make
source devel/setup.bash
rosrun mie443_contest1 contest1


*/
#include "callback_functions.cpp" // callback_functions.h doesn't work ...
// I should do more coding that's besides header files and organization

// #include "move_functions.h"

//Rotate Function -- OLD
void rotateBot(){
    ros::NodeHandle nh_custom;
    ros::Publisher vel_pub_custom = nh_custom.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop", 1);
    geometry_msgs::Twist vel_rotate;

    vel_rotate.angular.z=0.8;
    vel_pub_custom.publish(vel_rotate);
    return;
}

void e_stop(int time) { // why do we have this?
    ros::NodeHandle nh_custom;
    ros::Publisher vel_pub_custom = nh_custom.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop", 1);
    geometry_msgs::Twist vel_forward;

    ros::Subscriber bumper_sub_2 = nh_custom.subscribe("mobile_base/events/bumper", 5, &bumperCallback);
    ros::Subscriber laser_sub_2 = nh_custom.subscribe("scan", 2, &laserCallback);
    ros::Subscriber odom_2 = nh_custom.subscribe("odom", 1, &odomCallback);    
    
    ros::Rate loop_rate_2(100);

    vel_forward.angular.z = 0;
    vel_forward.linear.x = 0;
    vel_forward.linear.y = 0;
    
    // Start Timer
    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();
    uint64_t secondsElapsed = 0;

    // Stop the robot until secondsElapsed > time
    while(secondsElapsed < time){
        vel_pub_custom.publish(vel_forward);
        secondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()-start).count();
    }


}
/* RANDOM WALK SKELETON

while:

0. spinOnce
1. Check if way is clear
2. IF CLEAR: Move forward X meters
3. ELSE: Rotate 30 degrees

*/
// Call if bumpers are triggered
// Call if 

// 

// Checks all the sensors. Returns true if way is clear. F
bool sensor_check(double check_dist){
    
    if (minLaserDist < check_dist){

        return true;
    }

    else{
        return false;
    }

}

void move_x_meters(double meters){
    // Define Publishers and Subscribers
    // Take current position reading from odom
    // Travel backwards until distance travelled is meters long
    // Stop
    // End function

    /* Consider deleting these and only using subscribers/publsihers from int main */
    ros::NodeHandle nh_custom;
    ros::Publisher vel_pub_custom = nh_custom.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop", 1);
    geometry_msgs::Twist vel_forward;

    ros::Subscriber bumper_sub_2 = nh_custom.subscribe("mobile_base/events/bumper", 5, &bumperCallback);
    ros::Subscriber laser_sub_2 = nh_custom.subscribe("scan", 2, &laserCallback);
    ros::Subscriber odom_2 = nh_custom.subscribe("odom", 1, &odomCallback);    
    
    ros::Rate loop_rate_2(100);

    // Take current odom reading
    float old_x = posX;
    float old_y = posY;
    float init_pose[] = {old_x,old_y};
    double old_dist = 0.0;
    double distance = 0.0;

    // Keep moving 
    while (distance < meters){
        // Must spin to update odom values
        ros::spinOnce(); // More efficient way? maybe just ask odom_2 for info
        // Can we somehow wait here until odom_2 returns a value?

        distance = sqrt(pow(init_pose[0]-posX,2.0) + pow(init_pose[1]-posY,2.0));

        old_dist = distance;

        ROS_INFO("My distance: %f", distance);
        vel_forward.linear.x=-0.2;
        vel_pub_custom.publish(vel_forward);

        //Restricts loop rate to 100 Hz
        loop_rate_2.sleep();
        // For loop rate 5, distance = 0.345 m, but actual distance travelled = 0.5 m
        // For loop rate 2, distance = 0.285 m
        // For loop rate 100
    }

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener"); //can change the node name to anything
    ros::NodeHandle nh;

    // 2 subscribers
    ros::Subscriber bumper_sub = nh.subscribe("mobile_base/events/bumper", 10, &bumperCallback);
    ros::Subscriber laser_sub = nh.subscribe("scan", 10, &laserCallback);
    ros::Subscriber odom = nh.subscribe("odom", 1, &odomCallback);

    // 1 publisher
    ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop", 1);

    // Increasing it makes loops run more times per second.
    ros::Rate loop_rate(100);

    geometry_msgs::Twist vel;

    // contest count down timer
    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();
    uint64_t secondsElapsed = 0;

    float angular = 0.0;
    float linear = 0.0;

    int loopCount=0;

    while(ros::ok() && secondsElapsed <= 480) {
        ros::spinOnce();
        ROS_INFO("Min Laser: %f", minLaserDist);
        // ROS_INFO("Postion: (%f, %f) Orientation: %f degrees Range: %f", posX, posY, RAD2DEG(yaw), minLaserDist);
        // Check if any of the bumpers were pressed.
        bool any_bumper_pressed = false;
        for (uint32_t b_idx = 0; b_idx < N_BUMPER; ++b_idx) {
            any_bumper_pressed |= (bumper[b_idx] == kobuki_msgs::BumperEvent::PRESSED);
        }    

        // if (posX < 0.5 && yaw < M_PI / 12 && !any_bumper_pressed && minLaserDist > 0.7) {
        //     angular = 0.0;
        //     linear = 0.2;
        // }
        // else if (yaw < M_PI / 2 && posX > 0.5 && !any_bumper_pressed && minLaserDist > 0.5) {
        //     angular = M_PI / 6;
        //     linear = 0.0;
        // }
        // else if (minLaserDist > 1. && !any_bumper_pressed) {
        //     linear = 0.1;
        //     if (yaw < 17 / 36 * M_PI || posX > 0.6) {
        //         angular = M_PI / 12.;
        //     }
        //     else if (yaw < 19 / 36 * M_PI || posX < 0.4) {
        //         angular = -M_PI / 12.;
        //     }
        //     else {
        //         angular = 0;
        //     }
        // }
        // else {
        //     angular = 0.0;
        //     linear = 0.0;
        // }

        if (!any_bumper_pressed && sensor_check(0.05)){
        linear = 0.2;
        angular = 0.0;
        }
        else{
            move_x_meters(0.5);
        }


        // if (loopCount > 3) {
        //     e_stop(10);

        // }
        vel.angular.z = angular;
        vel.linear.x = linear;
        vel_pub.publish(vel);

        // The last thing to do is to update the timer.
        secondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()-start).count();
        
        loop_rate.sleep();
        loopCount++;
    }

    return 0;
}




        // //
        // // Check if any of the bumpers were pressed.
        // bool any_bumper_pressed = false;
        // for (uint32_t b_idx = 0; b_idx < N_BUMPER; ++b_idx) {
        // any_bumper_pressed |= (bumper[b_idx] == kobuki_msgs::BumperEvent::PRESSED);
        // }
        // //
        // // Control logic after bumpers are being pressed.
        // if (posX < 0.5 && yaw < M_PI / 12 && !any_bumper_pressed) {
        //     angular = 0.0;
        //     linear = 0.2;
        // }
        // else if (yaw < M_PI / 2 && posX > 0.5 && !any_bumper_pressed) {
        //     angular = M_PI / 6;
        //     linear = 0.0;
        // }
        // else {
        //     angular = 0.0;
        //     linear = 0.0;
        // }
        // vel.angular.z = angular;
        // vel.linear.x = linear;        // ROS_INFO("Postion: (%f, %f) Orientation: %f degrees Range: %f", posX, posY, RAD2DEG(yaw), minLaserDist);
        
        
        // // Check if any of the bumpers were pressed.
        // bool any_bumper_pressed = false;
        // for (uint32_t b_idx = 0; b_idx < N_BUMPER; ++b_idx) {
        //     any_bumper_pressed |= (bumper[b_idx] == kobuki_msgs::BumperEvent::PRESSED);
        // }    

        // if (posX < 0.5 && yaw < M_PI / 12 && !any_bumper_pressed && minLaserDist > 0.7) {
        //     angular = 0.0;
        //     linear = 0.2;
        // }
        // else if (yaw < M_PI / 2 && posX > 0.5 && !any_bumper_pressed && minLaserDist > 0.5) {
        //     angular = M_PI / 6;
        //     linear = 0.0;
        // }
        // else if (minLaserDist > 1. && !any_bumper_pressed) {
        //     linear = 0.1;
        //     if (yaw < 17 / 36 * M_PI || posX > 0.6) {
        //         angular = M_PI / 12.;
        //     }
        //     else if (yaw < 19 / 36 * M_PI || posX < 0.4) {
        //         angular = -M_PI / 12.;
        //     }
        //     else {
        //         angular = 0;
        //     }
        // }
        // else {
        //     angular = 0.0;
        //     linear = 0.0;
        // }


        // // Check if any of the bumpers were pressed.
        // bool any_bumper_pressed = false;
        // for (uint32_t b_idx = 0; b_idx < N_BUMPER; ++b_idx) {
        // any_bumper_pressed |= (bumper[b_idx] == kobuki_msgs::BumperEvent::PRESSED);
        // }
        // //
        // // Control logic after bumpers are being pressed.
        // if (posX < 0.5 && yaw < M_PI / 12 && !any_bumper_pressed) {
        //     angular = 0.0;
        //     linear = 0.2;
        // }
        // else if (yaw < M_PI / 2 && posX > 0.5 && !any_bumper_pressed) {
        //     angular = M_PI / 6;
        //     linear = 0.0;
        // }
        // else {
        //     angular = 0.0;
        //     linear = 0.0;
        // }
        // vel.angular.z = angular;
        // vel.linear.x = linear;
        // vel_pub.publish(vel);




        // SCAN EXAMPLE
        // ROS_INFO("Postion: (%f, %f) Orientation: %f degrees Range: %f", posX, posY, RAD2DEG(yaw), minLaserDist);
        // // Check if any of the bumpers were pressed.
        // bool any_bumper_pressed = false;
        // for (uint32_t b_idx = 0; b_idx < N_BUMPER; ++b_idx) {
        //     any_bumper_pressed |= (bumper[b_idx] == kobuki_msgs::BumperEvent::PRESSED);
        // }    

        // if (posX < 0.5 && yaw < M_PI / 12 && !any_bumper_pressed && minLaserDist > 0.7) {
        //     angular = 0.0;
        //     linear = 0.2;
        // }
        // else if (yaw < M_PI / 2 && posX > 0.5 && !any_bumper_pressed && minLaserDist > 0.5) {
        //     angular = M_PI / 6;
        //     linear = 0.0;
        // }
        // else if (minLaserDist > 1. && !any_bumper_pressed) {
        //     linear = 0.1;
        //     if (yaw < 17 / 36 * M_PI || posX > 0.6) {
        //         angular = M_PI / 12.;
        //     }
        //     else if (yaw < 19 / 36 * M_PI || posX < 0.4) {
        //         angular = -M_PI / 12.;
        //     }
        //     else {
        //         angular = 0;
        //     }
        // }
        // else {
        //     angular = 0.0;
        //     linear = 0.0;
        // }
