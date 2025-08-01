#include "../include/lsb.h"
#include "../include/decoder.h"
#include "../include/reg.h"
#include "../include/mem.h"
#include "../include/rob.h"
#include "../include/rs.h"
#include "../include/utils.h"
#include "../include/alu.h"

int main() {
  //freopen("../sample/sample.data", "r",stdin);
  //freopen("../testcases/pi.data", "r",stdin);
  //freopen("../test.out", "w",stdout);
  int total_cycle = 0;
  cpu_sim::memory.input_inst();
  std::cerr << "input successfully\n";
  while (!cpu_sim::halt) {
    total_cycle++;
    cpu_sim::reg.update();
    cpu_sim::decoder.update();
    cpu_sim::rob.update();
    cpu_sim::rs.update();
    cpu_sim::lsb.update();
    cpu_sim::alu.update();

    cpu_sim::alu.run();
    cpu_sim::rs.run();
    cpu_sim::rob.run();
    cpu_sim::decoder.run();
    cpu_sim::lsb.run();
  }
  const auto ans = cpu_sim::reg.now_reg[10].val_ & 0xFF;
  std::cout << ans << std::endl;
  std::cerr << "Total Cycle: " << total_cycle << std::endl;
  std::cerr << "Predict:  Total:" << cpu_sim::total_predict << " / False:" << cpu_sim::false_predict <<
      " => Correct Rate: " << (1.0 - (static_cast<double>(cpu_sim::false_predict) / cpu_sim::total_predict)) * 100
      << "%" << std::endl;
  return 0;
}
