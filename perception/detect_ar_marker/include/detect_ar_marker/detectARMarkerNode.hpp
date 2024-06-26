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

#ifndef DETECT_AR_MARKER_HPP_
#define DETECT_AR_MARKER_HPP_

#include <vector>
#include <memory>
#include <map>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/aruco.hpp>
#include "cv_bridge/cv_bridge.h"


namespace detect_ar_marker
{

class detectARMarkerNode : public rclcpp::Node
{
public:
    explicit detectARMarkerNode(const rclcpp::NodeOptions & node_options);

private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_image_;
    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr sub_cam_info_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_debug_image_;

    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    void image_subscriber_callback(const sensor_msgs::msg::Image::SharedPtr msg);
    void cam_info_subscriber_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    std::map<int, float> markerSizes;
};
} // namespace detect_ar_marker

#endif