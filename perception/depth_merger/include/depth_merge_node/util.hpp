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

#ifndef DEPTH_MERGE_UTIL_HPP_
#define DEPTH_MERGE_UTIL_HPP_

#include <builtin_interfaces/msg/time.hpp>
#include <std_msgs/msg/header.hpp>

namespace depth_merge_node
{
namespace util
{
    using key_time = std::pair<int32_t, uint32_t>;

    key_time get_key(const builtin_interfaces::msg::Time& msg);
    key_time get_key(const std_msgs::msg::Header& msg);
}
}

#endif