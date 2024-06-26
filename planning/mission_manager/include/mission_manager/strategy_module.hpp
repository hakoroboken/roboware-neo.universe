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

#ifndef STRATEGY_MODULE_HPP_
#define STRATEGY_MODULE_HPP_

#include <string>
#include <map>
#include <tuple>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>

#include <geometry_msgs/msg/pose_stamped.hpp>
#include <rw_planning_msg/msg/task_action.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <rw_common_util/geometry.hpp>

#include "mission_manager/type.hpp"
#include "mission_manager/mission_graph_util.hpp"

namespace mission_manager
{
namespace strategy_module
{
    class StartStrategy : public RWStrategy
    {
    public:
        StartStrategy();
        void update(node_bin& node, debug_info& info) override;
    };

    class EndStrategy : public RWStrategy
    {
    public:
        EndStrategy();
        void update(node_bin& node, debug_info& info) override;
    };

    class SetPoseStrategy : public RWStrategy
    {
    public:
        SetPoseStrategy();
        void update(node_bin& node, debug_info& info) override;
    };

    class FindStrategy : public RWStrategy
    {
    public:
        FindStrategy();
        void update(node_bin& node, debug_info& info) override;
    private:
        std::chrono::system_clock::time_point start_time;
    };

    class WaitStrategy : public RWStrategy
    {
    public:
        WaitStrategy();
        void update(node_bin& node, debug_info& info) override;
    private:
        std::chrono::system_clock::time_point start_time;
    };

    class LoopStrategy : public RWStrategy
    {
    public:
        LoopStrategy();
        void update(node_bin& node, debug_info& info) override;
    private:
        uint64_t count;
    };
}
} // namespace mission_manager

#endif