#ifndef REG_H
#define REG_H

#include "../include/utils.h"

struct RegNode {
  u32 val_ = 0;
  bool busy_ = false;
  int Q_ = -1;
};

class Register {
public:
  RegNode now_reg[kRegSize];
  RegNode next_reg[kRegSize];

  Register();

  ~Register();

  void upd();
};

#endif //REG_H
