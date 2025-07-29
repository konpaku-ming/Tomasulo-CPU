#include "../include/decoder.h"
#include "../include/mem.h"
#include "../include/rob.h"
#include "../include/predictor.h"

namespace cpu_sim {
  Decoder::Decoder() = default;

  Decoder::~Decoder() = default;

  void Decoder::update() {
    now_freeze_ = next_freeze_;
  }

  void Decoder::clear() {
    now_freeze_ = false;
    next_freeze_ = false;
  }

  void Decoder::fetch() {
    if (now_freeze_)return;
    if (rob.next_rob.full())return;
    upd_pc();
    const u32 code = memory.load(now_pc, 4);
    auto inst = decode(code, now_pc);
    //更新pc
    switch (inst.op) {
      case kJal:
        next_pc = now_pc + inst.imm;
        break;
      case kBeq:
      case kBge:
      case kBgeu:
      case kBlt:
      case kBltu:
      case kBne:
        //inst.predict = naive.predict(inst.pc);
        //inst.predict = two_bits.predict(inst.pc);
        inst.predict = gshare.predict(inst.pc);
        next_pc = inst.predict ? now_pc + inst.imm : now_pc + 4;
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
