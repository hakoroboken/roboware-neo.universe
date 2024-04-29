// Copyright 2024 Hakoroboken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PATH_FOLLOWER_NODE_HPP_
#define PATH_FOLLOWER_NODE_HPP_

#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/path.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>


namespace path_follower
{
    enum planner_status
    {
        ready,
        not_found,
        done,
        wip
    };

    class PathFollowerNode : public rclcpp::Node
    {
    public:
        explicit PathFollowerNode(const rclcpp::NodeOptions & node_options);

    private:
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_current_pose_;
        rclcpp::Subscription<nav_msgs::msg::Path>::SharedPtr sub_nav_path_;

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_twist_;

        rclcpp::TimerBase::SharedPtr control_timer_;

        geometry_msgs::msg::PoseStamped current_pose;
        nav_msgs::msg::Path global_path;
        planner_status pose_status;
        planner_status path_status;
        planner_status p_status;
        size_t start_pose_index;
        size_t target_pose_index;
        geometry_msgs::msg::PoseStamped start_pose;
        geometry_msgs::msg::PoseStamped goal_pose;


        void timer_callback();
        void current_pose_subscriber_callback(const geometry_msgs::msg::PoseStamped& sub_msg_pose);
        void nav_path_subscriber_callback(const nav_msgs::msg::Path& sub_msg_path);


    };
} // namespace path_follower

#endif