#include "../include/rs.h"

namespace cpu_sim {
  i32 alu_calc(OpType op, int vj, int vk, int imm) {
    // vj: pc/rs1
    int res;
    switch (op) {
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
}
