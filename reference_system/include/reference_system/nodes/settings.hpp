// Copyright 2021 Apex.AI, Inc.
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
#ifndef REFERENCE_SYSTEM__NODES__SETTINGS_HPP_
#define REFERENCE_SYSTEM__NODES__SETTINGS_HPP_

#include <chrono>
#include <string>
#include <vector>

namespace nodes
{

struct CommandSettings
{
  std::string node_name;
  std::string input_topic;
};

struct FusionSettings
{
  std::string node_name;
  std::string input_0;
  std::string input_1;
  std::string output_topic;
  std::chrono::nanoseconds number_crunch_time;
  std::chrono::nanoseconds max_input_time_difference;
};

struct TransformerSettings
{
  std::string node_name;
  std::string input_topic;
  std::string output_topic;
  std::chrono::nanoseconds number_crunch_time;
};

struct ReactorSettings
{
  std::string node_name;
  std::vector<std::string> inputs;
  std::string output_topic;
  std::chrono::nanoseconds cycle_time;
};

struct SensorSettings
{
  std::string node_name;
  std::string topic_name;
  std::chrono::nanoseconds cycle_time;
};
}  // namespace nodes
#endif  // REFERENCE_SYSTEM__NODES__SETTINGS_HPP_
