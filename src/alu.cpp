#include "../include/alu.h"
#include "../include/rs.h"

namespace cpu_sim {
  ArithmeticLogicUnit::ArithmeticLogicUnit() = default;

  ArithmeticLogicUnit::~ArithmeticLogicUnit() = default;

  void ArithmeticLogicUnit::update() {
    for (int i = 0; i < kRSSize; i++) {
      now_alu[i] = next_alu[i];
    }
  }

  void ArithmeticLogicUnit::clear() {
    for (int i = 0; i < kRSSize; i++) {
      now_alu[i].busy = false;
      next_alu[i].busy = false;
    }
  }


  u32 ArithmeticLogicUnit::alu_calc(const OpType op, const u32 vj, const u32 vk, const i32 a) {
    // auipc和jal用pc作为rs1
    u32 res;
    switch (op) {
      case kAdd:
        res = vj + vk;
        break;
      case kSub:
        res = vj - vk;
        break;
      case kAnd:
        res = vj & vk;
        break;
      case kOr:
        res = vj | vk;
        break;
      case kXor:
        res = vj ^ vk;
        break;
      case kSll:
        res = vj << vk;
        break;
      case kSrl:
        res = vj >> vk;
        break;
      case kSra:
        res = vj >> vk;
        res = static_cast<u32>(sign_extend(res, 32 - vk));
        break;
      case kSlt:
        res = static_cast<i32>(vj) < static_cast<i32>(vk) ? 1 : 0;
        break;
      case kSltu:
        res = vj < vk ? 1 : 0;
        break;
      case kAndi:
        res = vj & a;
        break;
      case kOri:
        res = vj | a;
        break;
      case kXori:
        res = vj ^ a;
        break;
      case kSlli:
        res = vj << static_cast<u32>(a);
        break;
      case kSrli:
        res = vj >> static_cast<u32>(a);
        break;
      case kSrai:
        res = vj >> static_cast<u32>(a);
        res = static_cast<u32>(sign_extend(res, 32 - a));
        break;
      case kSlti:
        res = static_cast<i32>(vj) < a ? 1 : 0;
        break;
      case kSltiu:
        res = vj < static_cast<u32>(a) ? 1 : 0;
        break;
      case kAuipc:
      case kAddi:
        res = vj + a;
        break;
      case kLui:
        res = a;
        break;
      //jal和jalr用返回的res作为pc
      case kJalr:
      case kJal:
        res = vj + a;
        break;
      //B类返回bool
      case kBeq:
        res = vj == vk ? 1 : 0;
        break;
      case kBge:
        res = static_cast<i32>(vj) >= static_cast<i32>(vk) ? 1 : 0;
        break;
      case kBgeu:
        res = vj >= vk ? 1 : 0;
        break;
      case kBlt:
        res = static_cast<i32>(vj) < static_cast<i32>(vk) ? 1 : 0;
        break;
      case kBltu:
        res = vj < vk ? 1 : 0;
        break;
      case kBne:
        res = vj != vk ? 1 : 0;
        break;
      default:
        std::cerr << "invalid op in RS: " << op << std::endl;
        assert(false);
    }
    return res;
  }

  void ArithmeticLogicUnit::run() {
    for (int i = 0; i < kRSSize; i++) {
      if (!now_alu[i].busy)continue;
      const auto cur = now_alu[i];
      //ALU运算
      next_alu[i].busy = false;
      rs.next_rs[i].res = alu_calc(cur.op, cur.vj, cur.vk, cur.a);
      rs.next_rs[i].busy = false;
    }
  }

  ArithmeticLogicUnit alu;
}
