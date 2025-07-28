#ifndef ROB_H
#define ROB_H

#include "inst.h"
#include "../include/utils.h"

namespace cpu_sim {
  enum ROBState {
    //下一步的操作
    kIssue,
    kExec,
    kWrite,
    kCommit,
  };

  struct ROBNode {
    ROBState status{};
    Inst inst;
    i32 dest = -1;
    i32 value = 0; //计算值
    u32 pos = 0;
    int progress = 0; //进度 0~3 用于处理某些沟槽的3周期指令

    ROBNode();

    explicit ROBNode(const Inst &i);
  };

  class ReorderBuffer {
  public:
    CirQue<ROBNode, kROBSize> now_rob;
    CirQue<ROBNode, kROBSize> next_rob;

    ReorderBuffer();

    ~ReorderBuffer();

    void upd();

    void clear();

    void run();

  private:
    void issue(int index);

    void write(int index);

    void commit();
  };

  extern ReorderBuffer rob;
}

#endif //ROB_H
