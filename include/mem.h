#ifndef MEM_H
#define MEM_H
#include <iostream>
#include <string>

struct Memory {
  u8 mem[0x200000];

  void input_instructions() {
    u32 load_pos = 0;
    std::string str;
    while (std::cin >> str) {
      if (str[0] == '@') {
        load_pos = std::stoul(str.substr(1), nullptr, 16);
      } else {
        mem[load_pos] = std::stoul(str, nullptr, 16);
        load_pos++;
      }
    }
  }

  void store(u32 val, const u32 pos, const u32 len) {
    for (u32 i = 0; i < len; i++) {
      mem[pos + i] = val & 0xFF; //取低8位
      val = val >> 8;
    }
  }
};

#endif //MEM_H
