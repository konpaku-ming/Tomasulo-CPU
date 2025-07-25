#include "../include/rob.h"

namespace cpu_sim {
  ROBNode::ROBNode() = default;

  ROBNode::ROBNode(const Inst &i) {
    inst = i;
    status = kIssue;
    dest = i.rd;
  }

  ReorderBuffer::ReorderBuffer() = default;

  ReorderBuffer::~ReorderBuffer() = default;


  void ReorderBuffer::upd() {
    now_rob = next_rob;
  }

  void ReorderBuffer::clear() {
    now_rob.clear();
    next_rob.clear();
  }

  ReorderBuffer rob;
}
