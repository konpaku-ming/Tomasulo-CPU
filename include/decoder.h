#ifndef DECODER_H
#define DECODER_H

#include "utils.h"

namespace cpu_sim {
  class Decoder {
  public:
    Decoder();

    ~Decoder();

    void upd();

    void fetch();

  private:
    bool now_freeze_ = false, next_freeze_ = false;
  };
}
#endif //DECODER_H
