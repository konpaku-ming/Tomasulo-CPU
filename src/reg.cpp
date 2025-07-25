#include "../include/reg.h"

namespace cpu_sim {
  Register::Register() {
    now_reg[0] = {0, -1};
    next_reg[0] = {0, -1};
  }

  Register::~Register() = default;

  void Register::upd() {
    for (int i = 0; i < kRegSize; ++i) {
      now_reg[i] = next_reg[i];
    }
  }

  Register reg;
}
