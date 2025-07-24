#ifndef RS_H
#define RS_H

#include "inst.h"
#include "utils.h"

struct RSNode {
  bool busy = false;
  Inst inst;
  int vj = 0, vk = 0;
  int qj = -1, qk = -1;
  int a = 0;
  int dest = -1;
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

  void execute();
};

#endif //RS_H
