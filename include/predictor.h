#ifndef PREDICTOR_H
#define PREDICTOR_H
#include "utils.h"

namespace cpu_sim {
  class NaivePredictor {
  public:
    NaivePredictor();

    ~NaivePredictor();

    bool predict(u32 pc);

    void update(u32 pc, int taken);
  };

  class TwoBitsPredictor {
  public:
    int log[1024]{};

    TwoBitsPredictor();

    ~TwoBitsPredictor();

    bool predict(u32 pc) const;

    void update(u32 pc, int taken);
  };

  class GSharePredictor {
  public:
    int log[1024]{};
    u32 history = 0;

    GSharePredictor();

    ~GSharePredictor();

    bool predict(u32 pc) const;

    void update(u32 pc, int taken);
  };

  extern NaivePredictor naive;

  extern TwoBitsPredictor two_bits;

  extern GSharePredictor gshare;
}


#endif //PREDICTOR_H
