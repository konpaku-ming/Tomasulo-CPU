#ifndef RS_H
#define RS_H

#include "inst.h"
#include "utils.h"

namespace cpu_sim {
  struct RSNode {
    bool busy = false;
    Inst inst;
    u32 vj = 0, vk = 0;
    int qj = -1, qk = -1;
    i32 a = 0;
    int rob_dest = -1;
    bool ready = false;
  };

  class ReservationStation {
  public:
    Arr<RSNode, kRSSize> now_rs;
    Arr<RSNode, kRSSize> next_rs;

    ReservationStation();

    ~ReservationStation();

    void upd();

    void clear();

    void run();
  };
}

#endif //RS_H
