#include "../include/reg.h"

namespace cpu_sim {
  Register::Register() {
    for (int i = 0; i < kRegSize; i++) {
      now_reg[i] = {0, -1};
      next_reg[i] = {0, -1};
    }
  }

  Register::~Register() = default;

  void Register::update() {
    for (int i = 0; i < kRegSize; ++i) {
      now_reg[i] = next_reg[i];
    }
  }

  void Register::clear() {
    for (int i = 0; i < kRegSize; i++) {
      now_reg[i].dep_ = -1;
      next_reg[i].dep_ = -1;
    }
  }

  Register reg;
}
