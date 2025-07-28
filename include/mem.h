#ifndef MEM_H
#define MEM_H
#include <iostream>
#include <string>
#include "../include/utils.h"

namespace cpu_sim {
  struct Memory {
    u8 mem[0x200000];

    void input_inst();

    void store(u32 val, u32 pos, u32 len);

    u32 load(u32 pos, u32 len) const;
  };

  extern Memory memory;
}

#endif //MEM_H
