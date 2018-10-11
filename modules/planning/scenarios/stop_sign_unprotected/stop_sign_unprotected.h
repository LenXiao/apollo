/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file
 **/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "modules/planning/proto/planning.pb.h"

#include "modules/common/status/status.h"
#include "modules/common/util/factory.h"
#include "modules/planning/common/speed_profile_generator.h"
#include "modules/planning/scenarios/scenario.h"

namespace apollo {
namespace planning {

class StopSignUnprotectedScenario : public Scenario {
 public:
  StopSignUnprotectedScenario() :
    Scenario(ScenarioConfig::STOP_SIGN_UNPROTECTED) {}
  virtual ~StopSignUnprotectedScenario() = default;

  bool Init() override;

  common::Status Process(const common::TrajectoryPoint& planning_init_point,
                         Frame* frame) override;

  bool IsTransferable(const Scenario& current_scenario,
                      const common::TrajectoryPoint& ego_point,
                      const Frame& frame) const override;

 private:
  enum StopSignUnprotectedStage {
    CRUISE_AND_MONITOR = 1,
    STOP = 2,
    CREEP = 3,
    INTERSECTION_CRUISE = 4,
  };

  void RegisterTasks();

  common::Status CruiseAndMonitor(Frame* frame);
  common::Status Stop(Frame* frame);
  common::Status Creep(
      const common::TrajectoryPoint& planning_start_point,
      Frame* frame);
  common::Status IntersectionCruise(
      const common::TrajectoryPoint& planning_start_point,
      Frame* frame);

 private:
  static int current_stage_index_;
  std::vector<std::unique_ptr<Task>> tasks_;
  ScenarioConfig config_;

  StopSignUnprotectedStage stage_ = CRUISE_AND_MONITOR;
  SpeedProfileGenerator speed_profile_generator_;
};

}  // namespace planning
}  // namespace apollo