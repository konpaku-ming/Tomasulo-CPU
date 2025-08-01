#include "../include/predictor.h"

namespace cpu_sim {
  NaivePredictor::NaivePredictor() = default;

  bool NaivePredictor::predict(const u32) const {
    return false;
  }

  void NaivePredictor::update(const u32, const int) {
  }

  TwoBitsPredictor::TwoBitsPredictor() = default;

  bool TwoBitsPredictor::predict(const u32 pc) const {
    auto hash = pc >> 2;
    hash = hash ^ (hash >> 10) ^ (hash >> 20);
    const auto idx = hash & 0x3FF;
    return log[idx] >= 2;
  }

  void TwoBitsPredictor::update(const u32 pc, const int taken) {
    auto hash = pc >> 2;
    hash = hash ^ (hash >> 10) ^ (hash >> 20);
    const auto idx = hash & 0x3FF;
    if (taken) {
      if (log[idx] < 3) log[idx]++;
    } else {
      if (log[idx] > 0) log[idx]--;
    }
  }

  GlobalPredictor::GlobalPredictor() = default;

  bool GlobalPredictor::predict(const u32 pc) const {
    auto hash = pc >> 2;
    hash = hash ^ (hash >> 10) ^ (hash >> 20);
    const auto idx = (hash ^ history) & 0x3FF;
    return log[idx] >= 2;
  }

  void GlobalPredictor::update(const u32 pc, const int taken) {
    auto hash = pc >> 2;
    hash = hash ^ (hash >> 10) ^ (hash >> 20);
    const auto idx = (hash ^ history) & 0x3FF;
    if (taken) {
      if (log[idx] < 3) log[idx]++;
    } else {
      if (log[idx] > 0) log[idx]--;
    }
    history = ((history << 1) | taken) & 0x3FF;
  }

  LocalPredictor::LocalPredictor() = default;

  bool LocalPredictor::predict(const u32 pc) const {
    auto hash = pc >> 2;
    hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & 0x3FF;
    const auto history = local_history[hash];
    const auto idx = (hash ^ history) & 0x3FF;
    return log[idx] >= 2;
  }

  void LocalPredictor::update(const u32 pc, const int taken) {
    auto hash = pc >> 2;
    hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & 0x3FF;
    const auto history = local_history[hash];
    const auto idx = (hash ^ history) & 0x3FF;
    if (taken) {
      if (log[idx] < 3) log[idx]++;
    } else {
      if (log[idx] > 0) log[idx]--;
    }
    local_history[hash] = ((history << 1) | taken) & 0x3FF;
  }

  ChoicePredictor::ChoicePredictor() = default;

  ChoicePredictor::~ChoicePredictor() = default;

  bool ChoicePredictor::prefer(const u32 pc) const {
    auto hash = pc >> 2;
    hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & 0x3FF;
    return choice[hash] >= 2; //true表示选global
  }

  void ChoicePredictor::update(const u32 pc, const bool local_correct, const bool global_correct) {
    auto hash = pc >> 2;
    hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & 0x3FF;
    if (global_correct && !local_correct && choice[hash] < 3) choice[hash]++;
    else if (!global_correct && local_correct && choice[hash] > 0) choice[hash]--;
  }

  TournamentPredictor::TournamentPredictor() = default;

  bool TournamentPredictor::predict(const u32 pc) const {
    return choice_predictor.prefer(pc) ? global_predictor.predict(pc) : local_predictor.predict(pc);
  }

  void TournamentPredictor::update(const u32 pc, const int taken) {
    const bool local_correct = local_predictor.predict(pc) == static_cast<bool>(taken);
    const bool global_correct = global_predictor.predict(pc) == static_cast<bool>(taken);
    choice_predictor.update(pc, local_correct, global_correct);
    local_predictor.update(pc, taken);
    global_predictor.update(pc, taken);
  }

  NaivePredictor naive;

  TwoBitsPredictor two_bits;

  GlobalPredictor global;

  LocalPredictor local;

  TournamentPredictor tournament;
}
