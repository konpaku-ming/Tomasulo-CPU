#ifndef DECODER_H
#define DECODER_H
#include "predictor.h"

namespace cpu_sim {
  class Decoder {
  public:
    Decoder();

    ~Decoder();

    void update();

    void clear();

    void run();

    void unfreeze();

  private:
    bool now_freeze_ = false, next_freeze_ = false;
  };

  extern Decoder decoder;

  extern Predictor *predictor;
}
#endif //DECODER_H
