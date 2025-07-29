#ifndef DECODER_H
#define DECODER_H

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
    int decode_cnt_ = 0;
  };

  extern Decoder decoder;
}
#endif //DECODER_H
