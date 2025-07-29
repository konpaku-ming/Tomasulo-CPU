#include "../include/decoder.h"
#include "../include/mem.h"
#include "../include/rob.h"

namespace cpu_sim {
  extern ReorderBuffer rob;
  extern Memory memory;

  Decoder::Decoder() = default;

  Decoder::~Decoder() = default;

  void Decoder::upd() {
    now_freeze_ = next_freeze_;
  }

  void Decoder::clear() {
    now_freeze_ = false;
    next_freeze_ = false;
  }

  void Decoder::fetch() {
    if (now_freeze_)return;
    if (rob.next_rob.full())return;
    decode_cnt_++;
    const u32 code = memory.load(now_pc, 4);
    const auto inst = decode(code, now_pc);
    //更新pc
    std::cerr << "op: " << inst.op << " | pc: " << std::hex << now_pc << std::dec << std::endl;
    switch (inst.op) {
      case kJal:
        //std::cerr << "decode JAL" << std::endl;
        next_pc = now_pc + inst.imm;
        break;
      case kBeq:
      case kBge:
      case kBgeu:
      case kBlt:
      case kBltu:
      case kBne:
        //TODO: Predictor
        next_pc = now_pc + 4;
        break;
      case kJalr:
      case kHalt:
        next_freeze_ = true;
        next_pc = now_pc;
        break;
      default:
        next_pc = now_pc + 4;
        break;
    }
    //进入ROB
    rob.next_rob.push(ROBNode(inst));
  }

  void Decoder::unfreeze() {
    next_freeze_ = false;
  }

  Decoder decoder;
}
