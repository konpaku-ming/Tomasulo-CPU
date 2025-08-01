#ifndef INST_H
#define INST_H

#include <cassert>
#include <iostream>
#include "../include/utils.h"

namespace cpu_sim {
  enum OpType {
    kInvalid = 0,
    kAdd, kSub, kAnd, kOr, kXor, kSll, kSrl, kSra, kSlt, kSltu,
    kAddi, kAndi, kOri, kXori, kSlli, kSrli, kSrai, kSlti, kSltiu,
    kLb, kLbu, kLh, kLhu, kLw,
    kSb, kSh, kSw,
    kBeq, kBge, kBgeu, kBlt, kBltu, kBne, kJal, kJalr,
    kAuipc, kLui,
    kHalt,
  };

  struct Inst {
    OpType op;
    u32 pc;
    i32 imm;
    i32 rs1, rs2, rd;
    bool predict;

    Inst() {
      op = kInvalid;
      pc = 0;
      imm = 0;
      rs1 = rs2 = rd = -1;
      predict = false;
    }
  };

  inline i32 sign_extend(const u32 value, const int bits) {
    const int shift = 32 - bits;
    return static_cast<i32>(value << shift) >> shift;
  }

  inline Inst decode(const u32 code, const u32 pc) {
    Inst inst;
    inst.pc = pc;
    if (code == 0x0FF00513) {
      //halt instruction
      inst.op = kHalt;
      return inst;
    }
    const u32 op = code & 0x7F; //取低7位
    switch (op) {
      case 0b0110111: {
        inst.op = kLui;
        inst.rd = (code >> 7) & 0x1F; //5位
        inst.imm = static_cast<i32>(code & 0xFFFFF000); //imm[31:12]
        break;
      }

      case 0b0010111: {
        inst.op = kAuipc;
        inst.rd = (code >> 7) & 0x1F; //5位
        inst.imm = static_cast<i32>(code & 0xFFFFF000); //imm[31:12]
        break;
      }

      case 0b1101111: {
        inst.op = kJal;
        inst.rd = (code >> 7) & 0x1F; //5位
        const u32 imm = (code >> 31 & 1) << 20 | (code >> 12 & 0xFF) << 12 |
                        (code >> 20 & 1) << 11 | (code >> 21 & 0x3FF) << 1;
        inst.imm = sign_extend(imm, 21);
        break;
      }

      case 0b1100111: {
        inst.op = kJalr;
        inst.rd = (code >> 7) & 0x1F; //5位
        inst.rs1 = (code >> 15) & 0x1F; //5位
        inst.imm = sign_extend(code >> 20, 12);
        break;
      }

      case 0b1100011: {
        inst.rs1 = code >> 15 & 0x1F;
        inst.rs2 = code >> 20 & 0x1F;
        switch (code >> 12 & 7) {
          case 0b000:
            inst.op = kBeq;
            break;
          case 0b001:
            inst.op = kBne;
            break;
          case 0b100:
            inst.op = kBlt;
            break;
          case 0b101:
            inst.op = kBge;
            break;
          case 0b110:
            inst.op = kBltu;
            break;
          case 0b111:
            inst.op = kBgeu;
            break;
          default:
            inst.op = kInvalid;
            std::cerr << "op mismatch" << std::endl;
            assert(false);
        }
        const u32 imm = (code >> 31 & 1) << 12 | (code >> 7 & 1) << 11
                        | (code >> 25 & 0x3F) << 5 | (code >> 8 & 0xF) << 1;
        inst.imm = sign_extend(imm, 13);
        break;
      }

      case 0b0000011: {
        inst.rd = (code >> 7) & 0x1F; //5位
        inst.rs1 = code >> 15 & 0x1F;
        switch (code >> 12 & 7) {
          case 0b000:
            inst.op = kLb;
            break;
          case 0b001:
            inst.op = kLh;
            break;
          case 0b010:
            inst.op = kLw;
            break;
          case 0b100:
            inst.op = kLbu;
            break;
          case 0b101:
            inst.op = kLhu;
            break;
          default:
            inst.op = kInvalid;
            std::cerr << "op mismatch" << std::endl;
            assert(false);
        }
        inst.imm = sign_extend(code >> 20, 12);
        break;
      }

      case 0b0100011: {
        inst.rs1 = code >> 15 & 0x1F;
        inst.rs2 = code >> 20 & 0x1F;
        switch (code >> 12 & 7) {
          case 0b000:
            inst.op = kSb;
            break;
          case 0b001:
            inst.op = kSh;
            break;
          case 0b010:
            inst.op = kSw;
            break;
          default:
            inst.op = kInvalid;
            std::cerr << "op mismatch" << std::endl;
            assert(false);
        }
        const u32 imm = (code >> 25 & 0x7F) << 5 | (code >> 7 & 0x1F);
        inst.imm = sign_extend(imm, 12);
        break;
      }

      case 0b0010011: {
        inst.rd = (code >> 7) & 0x1F; //5位
        inst.rs1 = code >> 15 & 0x1F;
        inst.imm = sign_extend(code >> 20, 12);
        switch (code >> 12 & 7) {
          case 0b000:
            inst.op = kAddi;
            break;
          case 0b010:
            inst.op = kSlti;
            break;
          case 0b011:
            inst.op = kSltiu;
            break;
          case 0b100:
            inst.op = kXori;
            break;
          case 0b110:
            inst.op = kOri;
            break;
          case 0b111:
            inst.op = kAndi;
            break;
          case 0b001:
            inst.op = kSlli;
            inst.imm = code >> 20 & 0x1F;
            break;
          case 0b101:
            inst.op = (code >> 30 & 1) ? kSrai : kSrli;
            inst.imm = code >> 20 & 0x1F;
            break;
          default:
            inst.op = kInvalid;
            std::cerr << "op mismatch" << std::endl;
            assert(false);
        }
        break;
      }

      case 0b0110011: {
        inst.rd = (code >> 7) & 0x1F; //5位
        inst.rs1 = code >> 15 & 0x1F;
        inst.rs2 = code >> 20 & 0x1F;
        switch (code >> 12 & 7) {
          case 0b000:
            inst.op = (code >> 30 & 1) ? kSub : kAdd;
            break;
          case 0b001:
            inst.op = kSll;
            break;
          case 0b010:
            inst.op = kSlt;
            break;
          case 0b011:
            inst.op = kSltu;
            break;
          case 0b100:
            inst.op = kXor;
            break;
          case 0b101:
            inst.op = (code >> 30 & 1) ? kSra : kSrl;
            break;
          case 0b110:
            inst.op = kOr;
            break;
          case 0b111:
            inst.op = kAnd;
            break;
          default:
            inst.op = kInvalid;
            std::cerr << "op mismatch" << std::endl;
            assert(false);
        }
        break;
      }
      default:
        inst.op = kInvalid;
        std::cerr << "op mismatch" << std::endl;
        assert(false);
    }
    return inst;
  }
}
#endif //INST_H
