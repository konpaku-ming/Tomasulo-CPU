#include "../include/reg.h"

namespace cpu_sim {
  Register::Register() {
    for (int i = 0; i < kRegSize; i++) {
      now_reg[i] = {0, -1};
      next_reg[i] = {0, -1};
    }
  }

  Register::~Register() = default;

  void Register::upd() {
    for (int i = 0; i < kRegSize; ++i) {
      now_reg[i] = next_reg[i];
    }
    now_reg[0] = next_reg[0] = {0, -1};
  }

  void Register::clear() {
    for (int i = 0; i < kRegSize; i++) {
      now_reg[i].dep_ = -1;
      next_reg[i].dep_ = -1;
    }
  }

  Register reg;
}
