#include "../include/reg.h"

Register::Register() {
  now_reg[0] = {0, false, -1};
  next_reg[0] = {0, false, -1};
}

Register::~Register() = default;

void Register::upd() {
  for (int i = 0; i < kRegSize; ++i) {
    now_reg[i] = next_reg[i];
  }
  now_reg[0] = {0, false, -1};
  next_reg[0] = {0, false, -1};
}
