#include "../include/predictor.h"

namespace cpu_sim {
  GSharePredictor::GSharePredictor() = default;

  GSharePredictor::~GSharePredictor() = default;

  bool GSharePredictor::predict(const u32 pc) const {
    const auto idx = ((pc >> 2) ^ history) & 0xFF;
    return log[idx] >= 2;
  }

  void GSharePredictor::update(const u32 pc, const int taken) {
    const auto idx = ((pc >> 2) ^ history) & 0xFF;
    if (taken) {
      if (log[idx] < 3) log[idx]++;
    } else {
      if (log[idx] > 0) log[idx]--;
    }
    history = ((history << 1) | taken) & 0xFF;
  }

  GSharePredictor gshare;
}
