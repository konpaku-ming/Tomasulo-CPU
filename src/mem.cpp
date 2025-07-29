#include "../include/mem.h"

namespace cpu_sim {
  void Memory::input_inst() {
    u32 load_pos = 0;
    std::string str;
    int cnt = 0;
    while (std::cin >> str) {
      if (str[0] == '@') {
        load_pos = std::stoul(str.substr(1), nullptr, 16);
      } else {
        mem[load_pos] = std::stoul(str, nullptr, 16);
        cnt++;
        load_pos++;
      }
    }
    if (cnt % 4) {
      std::cerr << "inst load error" << std::endl;
      assert(false);
    }
    std::cerr << "inst_num: " << cnt / 4 << std::endl;
  }

  void Memory::store(u32 val, const u32 pos, const u32 len) {
    for (auto i = 0; i < len; i++) {
      mem[pos + i] = val & 0xFF; //取低8位
      val = val >> 8;
    }
  }

  u32 Memory::load(const u32 pos, const u32 len) const {
    u32 ans = 0x00000000;
    for (auto i = 0; i < len; i++) {
      ans = ans | (static_cast<u32>(mem[pos + i]) << (i * 8));
    }
    return ans;
  }

  Memory memory;
}
