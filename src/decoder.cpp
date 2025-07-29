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
    std::cerr << "op: " << inst.op << " | pc: " << now_pc << std::endl;
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
        //TODO: Predictor
        //std::cerr << "decode BRANCH: " << inst.op << std::endl;
        break;
      case kJalr:
      case kHalt:
        //std::cerr << "decode JALR/HALT: " << inst.op << std::endl;
        next_freeze_ = true;
        next_pc = now_pc;
        break;
      default:
        //std::cerr << "decode normal inst: " << inst.op << std::endl;
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
