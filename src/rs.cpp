#include "../include/rs.h"
#include "../include/alu.h"
#include "../include/rob.h"

namespace cpu_sim {
  ReservationStation::ReservationStation() = default;

  ReservationStation::~ReservationStation() = default;

  void ReservationStation::update() {
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
      //检查依赖
      const auto cur = now_rs[i];
      //ALU运算
      if (cur.busy) {
        if (cur.inst.op == kHalt) {
          const auto dest = cur.rob_dest;
          rob.next_rob[dest].progress = 3; //一周期
          rob.next_rob[dest].status = kWrite;
          //无计算
          next_rs.remove(i);
          return;
        }
        if (cur.qj != -1 || cur.qk != -1)continue;
        if (!alu.next_alu[i].busy) {
          const AluNode node = {cur.inst.op, cur.vj, cur.vk, cur.a};
          alu.next_alu[i] = node;
          alu.next_alu[i].busy = true;
        }
      } else {
        const auto dest = cur.rob_dest;
        rob.next_rob[dest].progress = 3; //一周期
        if (cur.inst.op == kJal || cur.inst.op == kJalr) {
          rob.next_rob[dest].pos = cur.res; //新pc作为pos值返回
          rob.next_rob[dest].value = cur.inst.pc + 4;
        } else {
          rob.next_rob[dest].value = cur.res;
        }
        rob.next_rob[dest].status = kWrite;
        next_rs.remove(i);
      }
    }
  }

  ReservationStation rs;
}
