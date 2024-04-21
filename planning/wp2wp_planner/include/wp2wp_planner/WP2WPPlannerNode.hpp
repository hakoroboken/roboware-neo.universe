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

#ifndef WP2WP_PLANNER_NODE_HPP
#define WP2WP_PLANNER_NODE_HPP

#include <vector>
#include <memory>
#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <nav_msgs/msg/path.hpp>

#include <tf2/LinearMath/Quaternion.h>

namespace wp2wp_planner
{
    class WP2WPPlannerNode : public rclcpp::Node
    {
    public:
        explicit WP2WPPlannerNode(const rclcpp::NodeOptions & node_options);

    private:
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_current_pose_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_goal_pose_;

        rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr pub_global_plan_path_;

        geometry_msgs::msg::PoseStamped current_pose;

        void current_pose_subscriber_callback(const geometry_msgs::msg::PoseStamped& msg);
        void goal_pose_subscriber_callback(const geometry_msgs::msg::PoseStamped& msg);

    };
} // namespace wp2wp_planner

#endif