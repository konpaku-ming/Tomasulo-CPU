#include "../include/lsb.h"
#include "../include/decoder.h"
#include "../include/reg.h"
#include "../include/mem.h"
#include "../include/rob.h"
#include "../include/rs.h"
#include "../include/utils.h"

int main() {
  freopen("../testcases/qsort.data", "r",stdin);
  //freopen("../testcases/array_test1.data", "r",stdin);
  //freopen("../sample/sample.data", "r",stdin);
  freopen("../test.out", "w",stdout);
  int total_cycle = 0;
  cpu_sim::memory.input_inst();
  std::cerr << "input successfully\n";
  while (!cpu_sim::halt) {
    total_cycle++;
    cpu_sim::upd_pc();
    cpu_sim::reg.upd();
    cpu_sim::decoder.upd();
    cpu_sim::rob.upd();
    cpu_sim::rs.upd();
    cpu_sim::lsb.upd();

    cpu_sim::decoder.fetch();
    cpu_sim::rob.run();
    cpu_sim::rs.run();
    cpu_sim::lsb.run();

    if (total_cycle == 10000000)
      assert(false);
  }
  const auto ans = cpu_sim::reg.now_reg[10].val_ & 0xFF;
  std::cout << ans << std::endl;
  std::cerr << "Time consume: " << total_cycle << std::endl;
  return 0;
}
