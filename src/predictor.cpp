#include "../include/predictor.h"

namespace cpu_sim {
  NaivePredictor::NaivePredictor() = default;

  NaivePredictor::~NaivePredictor() = default;

  bool NaivePredictor::predict(const u32) {
    return false;
  }

  void NaivePredictor::update(const u32, const int) {
  }

  TwoBitsPredictor::TwoBitsPredictor() = default;

  TwoBitsPredictor::~TwoBitsPredictor() = default;

  bool TwoBitsPredictor::predict(const u32 pc) const {
    const auto idx = (pc >> 2) & 0x3FF;
    return log[idx] >= 2;
  }

  void TwoBitsPredictor::update(const u32 pc, const int taken) {
    const auto idx = (pc >> 2) & 0x3FF;
    if (taken) {
      if (log[idx] < 3) log[idx]++;
    } else {
      if (log[idx] > 0) log[idx]--;
    }
  }

  GSharePredictor::GSharePredictor() = default;

  GSharePredictor::~GSharePredictor() = default;

  bool GSharePredictor::predict(const u32 pc) const {
    const auto idx = ((pc >> 2) ^ history) & 0x3FF;
    return log[idx] >= 2;
  }

  void GSharePredictor::update(const u32 pc, const int taken) {
    const auto idx = ((pc >> 2) ^ history) & 0x3FF;
    if (taken) {
      if (log[idx] < 3) log[idx]++;
    } else {
      if (log[idx] > 0) log[idx]--;
    }
    history = ((history << 1) | taken) & 0x3FF;
  }

  NaivePredictor naive;

  TwoBitsPredictor two_bits;

  GSharePredictor gshare;
}
