#ifndef PREDICTOR_H
#define PREDICTOR_H
#include "utils.h"

namespace cpu_sim {
  class GSharePredictor {
  public:
    int log[256]{};
    u32 history = 0;

    GSharePredictor();

    ~GSharePredictor();

    bool predict(u32 pc) const;

    void update(u32 pc, int taken);
  };

  extern GSharePredictor gshare;
}


#endif //PREDICTOR_H
