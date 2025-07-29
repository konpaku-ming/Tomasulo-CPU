#include "../include/lsb.h"
#include "../include/mem.h"
#include "../include/rob.h"

namespace cpu_sim {
  LoadStoreBuffer::LoadStoreBuffer() = default;

  LoadStoreBuffer::~LoadStoreBuffer() = default;

  void LoadStoreBuffer::upd() {
    now_lsb = next_lsb;
    now_freeze = next_freeze;
  }

  void LoadStoreBuffer::clear() {
    now_lsb.clear();
    next_lsb.clear();
    now_freeze = false;
    next_freeze = false;
  }

  void LoadStoreBuffer::run() {
    //完全顺序
    if (now_lsb.empty())return;
    const auto cur = now_lsb.front();
    std::cerr << "LSB front: " << cur.inst.op << std::endl;
    if (cur.qj != -1 || cur.qk != -1)return;
    std::cerr << "**LSB front: **" << cur.inst.op << std::endl;
    const auto pos = cur.vj + cur.a; //rs1 + imm
    const int dest = cur.rob_dest;
    switch (cur.inst.op) {
      case kLb: {
        if (rob.next_rob[dest].progress == 0) {
          if (now_freeze)return;
          rob.next_rob[dest].progress = 1;
          return;
        }
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = sign_extend(memory.load(pos, 1), 8);
          rob.next_rob[dest].value = val;
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      }
      case kLh: {
        if (rob.next_rob[dest].progress == 0) {
          if (now_freeze)return;
          rob.next_rob[dest].progress = 1;
          return;
        }
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = sign_extend(memory.load(pos, 2), 16);
          rob.next_rob[dest].value = val;
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      }
      case kLw: {
        if (rob.next_rob[dest].progress == 0) {
          if (now_freeze)return;
          rob.next_rob[dest].progress = 1;
          return;
        }
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = sign_extend(memory.load(pos, 4), 32);
          rob.next_rob[dest].value = val;
          std::cerr << "load word: " << val << "------" << std::endl;
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      }
      case kLbu: {
        if (rob.next_rob[dest].progress == 0) {
          if (now_freeze)return;
          rob.next_rob[dest].progress = 1;
          return;
        }
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = memory.load(pos, 1);
          rob.next_rob[dest].value = static_cast<i32>(val);
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      }
      case kLhu: {
        if (rob.next_rob[dest].progress == 0) {
          if (now_freeze)return;
          rob.next_rob[dest].progress = 1;
          return;
        }
        rob.next_rob[dest].progress++;
        if (rob.next_rob[dest].progress == 3) {
          const auto val = memory.load(pos, 2);
          rob.next_rob[dest].value = static_cast<i32>(val);
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      }
      case kSb:
      case kSh:
      case kSw: {
        if (rob.next_rob[dest].progress == 0) {
          if (now_freeze)return;
          rob.next_rob[dest].progress = 1;
          break;
        }
        rob.next_rob[dest].progress++;
        //std::cerr << cur.inst.op << " progress: " << rob.next_rob[dest].progress << std::endl;
        next_freeze = true;
        if (rob.next_rob[dest].progress == 3) {
          //在commit时才能修改RAM
          rob.next_rob[dest].pos = pos;
          rob.next_rob[dest].value = static_cast<i32>(cur.vk);
          rob.next_rob[dest].status = kWrite;
          next_lsb.pop();
        }
        break;
      }
      default:
        assert(false);
    }
  }

  void LoadStoreBuffer::unfreeze() {
    next_freeze = false;
  }

  LoadStoreBuffer lsb;
}
