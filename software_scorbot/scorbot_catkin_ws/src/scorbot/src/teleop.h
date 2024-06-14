#ifndef SCORBOT_H
#define SCORBOT_H

#include <ros/ros.h>
#include <universal_teleop/Control.h>
#include <universal_teleop/Event.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <sensor_msgs/JointState.h>
#include <scorbot/JointVelocities.h>


namespace scorbot {
  class Teleop
  {
    public:
      Teleop(ros::NodeHandle& n);

      ros::Subscriber joint_trajectory_sub;
      //ros::Publisher joint_trajectory_pub;
      ros::Subscriber joint_states_sub;
      ros::Publisher joint_pos_array_pub;
      void on_trajectory(const trajectory_msgs::JointTrajectoryConstPtr& msg);
      void on_joint_states(const sensor_msgs::JointStatesConstPtr& msg);

      ros::Subscriber sub_events, sub_control;
      ros::Publisher vel_pub, home_pub;
      void on_controls(const universal_teleop::ControlConstPtr& msg);
      void on_events(const universal_teleop::EventConstPtr& msg);

      ros::Publisher claw_catch_pub, claw_release_pub;

      void on_control_cycle(const ros::TimerEvent& ev);

    private:
      int control_frequency;
      ros::Timer control_timer;
      bool override_enabled, slow_mode_enabled;

      std::vector<float> pos_juntas;

      std::vector<int> joint_trajectory_goals;
      std::vector<bool> reached_current_goal;
      int current_goal_index;
      int current_goal_length;

      scorbot::JointVelocities velocities;
      std::vector<int> joint_states;
  };
}

#endif // SCORBOT_H
