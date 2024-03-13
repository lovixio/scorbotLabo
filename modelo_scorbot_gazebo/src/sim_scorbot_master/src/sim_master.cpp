#include <std_msgs/Empty.h>
#include <math.h>
#include <scorbot/JointTrajectory.h>
#include "teleop.h"

#define MAX_TRAJECTORY_SIZE 10
#define NUM_JUNTAS 5

#define ENC2RAD1(x) ((x - 7000)  * (float)0.000034142)
#define ENC2RAD2(x) ((x - 22500) * (float)0.000030712)
#define ENC2RAD3(x) ((x - 16500) * (float)-0.000032903)
#define ENC2RAD4(x) ((x - 0)     * (float)-0.000054786)
#define ENC2RAD5(x) ((x - 2200)  * (float)-0.000163399)

#define RAD2ENC1(x) ((int32_t)(x / (float)0.000034142) + 7000)
#define RAD2ENC2(x) ((int32_t)(x / (float)0.000030712) + 22500)
#define RAD2ENC3(x) ((int32_t)(x / (float)-0.000032903) + 16500)
#define RAD2ENC4(x) ((int32_t)(x / (float)-0.000054786) + 0)
#define RAD2ENC5(x) ((int32_t)(x / (float)-0.000163399) + 2200)

using namespace std;

scorbor::Sim_Master::Sim_Master(ros::NodeHandle& n){
    n.param("control_frequency", control_frequency, 5);

    joint_trajectory_sub = n.subscribe<trajectory_msgs::JointTrajectory>("/scorbot/joint_path_command", 1, &Teleop::on_trajectory, this);
  
    pub_base = n.publish<std_msgs::Float64>('/scorbot/base_position_controller/command', 1);
    pub_shoulder = n.publish<std_msgs::Float64>('/scorbot/shoulder_position_controller/command', 1);
    pub_elbow = n.publish<std_msgs::Float64>('/scorbot/elbow_position_controller/command', 1);
    pub_pitch = n.publish<std_msgs::Float64>('/scorbot/pitch_position_controller/command', 1);
    pub_roll = n.publish<std_msgs::Float64>('/scorbot/roll_position_controller/command', 1);

    int32_t pos_juntas[NUM_JUNTAS];
    char* joint_names[] = { "joint1", "joint2", "joint3", "joint4", "joint5" };

    int32_t joint_trajectory_goals[MAX_TRAJECTORY_SIZE][NUM_JUNTAS];
    bool reached_current_goal[NUM_JUNTAS] = { false, false, false, false, false };
    int current_goal_index = -1;
    int current_goal_length = 0;
}

void on_trajectory(const scorbot::JointTrajectory& trajectory)
{

  current_goal_length = trajectory.points_length / 5;

  if (current_goal_length > MAX_TRAJECTORY_SIZE || current_goal_length == 0) return;
  
  current_goal_index = 0;
  
  for (int i = 0; i < current_goal_length; i++)
  {
    joint_trajectory_goals[i][0] = trajectory.points[i*NUM_JUNTAS+0];
    joint_trajectory_goals[i][1] = trajectory.points[i*NUM_JUNTAS+1];
    joint_trajectory_goals[i][2] = trajectory.points[i*NUM_JUNTAS+2];
    joint_trajectory_goals[i][3] = trajectory.points[i*NUM_JUNTAS+3];
    joint_trajectory_goals[i][4] = trajectory.points[i*NUM_JUNTAS+4];
  }
  
  reached_current_goal[0] = false;
  reached_current_goal[1] = false;
  reached_current_goal[2] = false;
  reached_current_goal[3] = false;
  reached_current_goal[4] = false;  
  
  /* set initial point as goal */
  //cambiar esto por mandar los mensajes al simulador
  set_position(1, joint_trajectory_goals[0][0]);
  set_position(2, joint_trajectory_goals[0][1]);
  set_position(3, joint_trajectory_goals[0][2]);
  set_position(4, joint_trajectory_goals[0][3]);
  set_position(5, joint_trajectory_goals[0][4]);  
}