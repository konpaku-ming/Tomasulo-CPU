#include "../include/lsb.h"
#include "../include/decoder.h"
#include "../include/reg.h"
#include "../include/mem.h"
#include "../include/rob.h"
#include "../include/rs.h"
#include "../include/utils.h"

int main() {
  //freopen("../sample/sample.data", "r",stdin);
  //freopen("../testcases/bulgarian.data", "r",stdin);
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

    cpu_sim::rs.run();
    cpu_sim::rob.run();
    cpu_sim::decoder.fetch();
    cpu_sim::lsb.run();
  }
  const auto ans = cpu_sim::reg.now_reg[10].val_ & 0xFF;
  std::cout << ans << std::endl;
  std::cerr << "Time consume: " << total_cycle << std::endl;
  std::cerr << "Predict:  Total:" << cpu_sim::total_predict << " / False:" << cpu_sim::false_predict << std::endl;
  return 0;
}
