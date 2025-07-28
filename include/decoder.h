#ifndef DECODER_H
#define DECODER_H

#include "../include/utils.h"

namespace cpu_sim {
  class Decoder {
  public:
    Decoder();

    ~Decoder();

    void upd();

    void clear();

    void fetch();

    void unfreeze();

  private:
    bool now_freeze_ = false, next_freeze_ = false;
  };

  extern Decoder decoder;
}
#endif //DECODER_H
