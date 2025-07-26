#include "../include/rs.h"

namespace cpu_sim {
  u32 alu_calc(OpType op, u32 vj, u32 vk, i32 a) {
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
      case kLb:
      case kLh:
      case kLw:
      case kLbu:
      case kLhu:
      case kSb:
      case kSh:
      case kSw:
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
        std::cerr << "invalid op" << std::endl;
        assert(false);
    }
    return res;
  }

  ReservationStation::ReservationStation() = default;

  ReservationStation::~ReservationStation() = default;

  void ReservationStation::upd() {
    now_rs = next_rs;
  }

  void ReservationStation::clear() {
    now_rs.clear();
    next_rs.clear();
  }

  void ReservationStation::run() {
    if (now_rs.empty())return;
    for (int i = 0; i < kRSSize; i++) {
      if (!now_rs.exist(i))continue;
      //TODO 检查参数
      //TODO 执行，特判几个指令
    }
  }


  ReservationStation rs;
}
