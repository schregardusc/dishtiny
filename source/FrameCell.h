#pragma once

#include <algorithm>
#include <list>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"
#include "GeometryHelper.h"
#include "FrameHardware.h"

class FrameCell {

private:

  Manager &man;
  emp::Random &local_rng;
  const Config &cfg;

  emp::vector<emp::Ptr<FrameHardware>> hw;
  const emp::vector<size_t> neighs;
  const size_t pos;

public:

  FrameCell() = delete;

  FrameCell(
    emp::Random &local_rng_,
    const Config &cfg_,
    Manager &man_,
    const size_t pos_,
    const Config::inst_lib_t &inst_lib,
    const Config::event_lib_t &event_lib
  );

  ~FrameCell();

  void Reset();

  void Process(const size_t update);

  void SetProgram(const Config::program_t & program);

  size_t GetPos() const;

  size_t GetPauseSum(const size_t lev) const;

  Manager& Man();

  FrameHardware& GetFrameHardware(const size_t dir);

  size_t GetNeigh(const size_t dir) const;

  void QueueMessages(emp::vector<Config::inbox_t> &inboxes);

};
