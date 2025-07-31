#ifndef ALU_H
#define ALU_H

#include "inst.h"
#include "../include/utils.h"

namespace cpu_sim {
  struct AluNode {
    OpType op;
    u32 vj = 0, vk = 0;
    i32 a = 0;
    bool busy = false;
  };

  class ArithmeticLogicUnit {
  public:
    AluNode now_alu[kRSSize];
    AluNode next_alu[kRSSize];

    ArithmeticLogicUnit();

    ~ArithmeticLogicUnit();

    void update();

    void clear();

    void run();

  private:
    static u32 alu_calc(OpType op, u32 vj, u32 vk, i32 a);
  };

  extern ArithmeticLogicUnit alu;
}

#endif //ALU_H
