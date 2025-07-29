#include "../include/utils.h"

namespace cpu_sim {
  bool halt = false;
  u32 now_pc = 0;
  u32 next_pc = 0;
  int false_predict = 0;
  int total_predict = 0;
}
