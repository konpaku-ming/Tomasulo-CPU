#ifndef LSB_H
#define LSB_H

#include "inst.h"
#include "utils.h"

namespace cpu_sim {
  struct LSBNode {
    Inst inst;
    u32 vj = 0, vk = 0;
    int qj = -1, qk = -1;
    i32 a = 0;
    int rob_dest = -1;
  };

  class LoadStoreBuffer {
  public:
    CirQue<LSBNode, kLSBSize> now_lsb;
    CirQue<LSBNode, kLSBSize> next_lsb;
    bool now_freeze = false;
    bool next_freeze = false;

    LoadStoreBuffer();

    ~LoadStoreBuffer();

    void update();

    void clear();

    void run();

    void unfreeze();
  };

  extern LoadStoreBuffer lsb;
}

#endif //LSB_H
