#include "../include/lsb.h"
#include "../include/mem.h"
#include "../include/rob.h"

namespace cpu_sim {
  extern ReorderBuffer rob;
  extern Memory memory;

  LoadStoreBuffer::LoadStoreBuffer() = default;

  LoadStoreBuffer::~LoadStoreBuffer() = default;

  void LoadStoreBuffer::upd() {
    now_lsb = next_lsb;
  }

  void LoadStoreBuffer::clear() {
    now_lsb.clear();
    next_lsb.clear();
    freeze = false;
  }

  void LoadStoreBuffer::run() {
    //完全顺序
    if (now_lsb.empty() || freeze)return;
    const auto cur = now_lsb.front();
    if (cur.qj != -1 || cur.qk != -1)return;
    const auto pos = cur.vj + cur.a; //rs1 + imm
    const int dest = cur.rob_dest;
    switch (cur.inst.op) {
      case kLb:
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = sign_extend(memory.load(pos, 1), 8);
          rob.next_rob[dest].value = static_cast<u32>(val);
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      case kLh:
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = sign_extend(memory.load(pos, 2), 16);
          rob.next_rob[dest].value = static_cast<u32>(val);
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      case kLw:
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = sign_extend(memory.load(pos, 4), 32);
          rob.next_rob[dest].value = static_cast<u32>(val);
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      case kLbu:
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = memory.load(pos, 1);
          rob.next_rob[dest].value = val;
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      case kLhu:
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = memory.load(pos, 2);
          rob.next_rob[dest].value = val;
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      case kSb:
      case kSh:
      case kSw:
        rob.next_rob[dest].progress++;
        freeze = true;
        if (rob.next_rob[dest].progress == 3) {
          //在commit时才能修改RAM
          rob.next_rob[dest].pos = pos;
          rob.next_rob[dest].value = cur.vk;
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      default:
        assert(false);
    }
  }

  LoadStoreBuffer lsb;
}
