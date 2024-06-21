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


#include "detect_ar_marker/detectARMarkerNode.hpp"

namespace detect_ar_marker
{
    detectARMarkerNode::detectARMarkerNode(const rclcpp::NodeOptions &node_option)
        : rclcpp::Node("detect_ar_marker_node", node_option)
    {   
      sub_image_ = create_subscription<sensor_msgs::msg::Image>(
        "input/raw_image", 0, std::bind(&detectARMarkerNode::image_subscriber_callback, this, std::placeholders::_1));

      sub_cam_info_ = create_subscription<sensor_msgs::msg::CameraInfo>(
        "input/camera_info", 0, std::bind(&detectARMarkerNode::cam_info_subscriber_callback, this, std::placeholders::_1));

      pub_debug_image_ = create_publisher<sensor_msgs::msg::Image>(
        "debug/image", 0);

      tf_broadcaster_ =
        std::make_unique<tf2_ros::TransformBroadcaster>(*this);

      for(auto i = 0; i < 100; ++i)
      {
        auto param_name = "marker_length." + std::to_string(i);
        markerSizes[i] = declare_parameter<double>(param_name, 0.1);
      }

      cameraMatrix = (cv::Mat_<double>(3, 3) << 0.0, 0.0, 0.0,
                                                0.0, 0.0, 0.0,
                                                0.0, 0.0, 1.0);

      distCoeffs = (cv::Mat_<double>(5, 1) << 0.0, 0.0,
                                              0.0, 0.0,
                                              0.0);
    }

    void detectARMarkerNode::image_subscriber_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
      auto cv_image = cv_bridge::toCvShare(msg, msg->encoding);
      std::vector<int> markerIds;
      std::vector<std::vector<cv::Point2f>> markerCorners;
      auto dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);
      cv::Ptr<cv::aruco::Dictionary> dictPtr = cv::makePtr<cv::aruco::Dictionary>(dictionary);
      cv::aruco::detectMarkers(cv_image->image, dictPtr, markerCorners, markerIds);

      std::vector<cv::Vec3d> rvecs, tvecs;
      if(markerIds.size() > 0){
          cv::aruco::drawDetectedMarkers(cv_image->image, markerCorners, markerIds);

          // マーカーごとに姿勢推定を行うためのループ
          for(size_t i = 0; i < markerIds.size(); i++) {
              int id = markerIds[i];
              
              // マーカーの大きさを取得
              if (markerSizes.find(id) == markerSizes.end()) {
                  markerSizes[id] = 0.1;
              }
              float markerLength = markerSizes[id];

              std::vector<std::vector<cv::Point2f>> singleMarkerCorners = { markerCorners[i] };
              std::vector<cv::Vec3d> singleRvecs, singleTvecs;

              cv::aruco::estimatePoseSingleMarkers(singleMarkerCorners, markerLength, cameraMatrix, distCoeffs, singleRvecs, singleTvecs);

              rvecs.push_back(singleRvecs[0]);
              tvecs.push_back(singleTvecs[0]);
              
              geometry_msgs::msg::TransformStamped transformStamped;

              transformStamped.header.stamp = now();
              transformStamped.header.frame_id = msg->header.frame_id;
              transformStamped.child_frame_id = std::to_string(id);
              transformStamped.transform.translation.x = singleTvecs[0][0];
              transformStamped.transform.translation.y = singleTvecs[0][1];
              transformStamped.transform.translation.z = singleTvecs[0][2];

              cv::Mat rotMat;
              cv::Rodrigues(singleRvecs[0], rotMat);

              tf2::Matrix3x3 tfRotMat(
                  rotMat.at<double>(0, 0), rotMat.at<double>(0, 1), rotMat.at<double>(0, 2),
                  rotMat.at<double>(1, 0), rotMat.at<double>(1, 1), rotMat.at<double>(1, 2),
                  rotMat.at<double>(2, 0), rotMat.at<double>(2, 1), rotMat.at<double>(2, 2)
              );

              tf2::Quaternion q;
              tfRotMat.getRotation(q);

              transformStamped.transform.rotation.x = q.x();
              transformStamped.transform.rotation.y = q.y();
              transformStamped.transform.rotation.z = q.z();
              transformStamped.transform.rotation.w = q.w();

              tf_broadcaster_->sendTransform(transformStamped);
          }
      }

      auto ros_img_msg = sensor_msgs::msg::Image();
      cv_image->toImageMsg(ros_img_msg);
      pub_debug_image_->publish(ros_img_msg);

    }

    void detectARMarkerNode::cam_info_subscriber_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg){

      msg->k[0];
      cameraMatrix = (cv::Mat_<double>(3, 3) << msg->k[0], msg->k[1], msg->k[2],
                                                msg->k[3], msg->k[4], msg->k[5],
                                                msg->k[6], msg->k[7], msg->k[8]);

      distCoeffs = (cv::Mat_<double>(5, 1) << msg->d[0], msg->d[1],
                                              msg->d[2], msg->d[3],
                                              msg->d[4]);
    }

}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(detect_ar_marker::detectARMarkerNode)