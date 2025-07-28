#ifndef REG_H
#define REG_H

#include "../include/utils.h"

namespace cpu_sim {
  struct RegNode {
    u32 val_ = 0;
    int dep_ = -1; //依赖
  };

  class Register {
  public:
    RegNode now_reg[kRegSize];
    RegNode next_reg[kRegSize];

    Register();

    ~Register();

    void upd();

    void clear();
  };

  extern Register reg;
}

#endif //REG_H
