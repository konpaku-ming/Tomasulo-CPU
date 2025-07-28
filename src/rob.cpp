#include "../include/rob.h"
#include "../include/decoder.h"
#include "../include/lsb.h"
#include "../include/mem.h"
#include "../include/reg.h"
#include "../include/rs.h"

namespace cpu_sim {
  ROBNode::ROBNode() = default;

  ROBNode::ROBNode(const Inst &i) {
    inst = i;
    status = kIssue;
    dest = i.rd;
    if (i.rd == 0)dest = -1;
    progress = 0;
  }

  ReorderBuffer::ReorderBuffer() = default;

  ReorderBuffer::~ReorderBuffer() = default;


  void ReorderBuffer::upd() {
    now_rob = next_rob;
  }

  void ReorderBuffer::clear() {
    now_rob.clear();
    next_rob.clear();
  }

  void ReorderBuffer::issue(const int index) {
    const auto cur = now_rob[index];
    switch (cur.inst.op) {
      case kLb:
      case kLbu:
      case kLh:
      case kLhu:
      case kLw:
      case kSb:
      case kSh:
      case kSw: {
        if (!lsb.next_lsb.full())break;
        LSBNode lsb_node;
        lsb_node.inst = cur.inst;
        //处理rs1
        if (cur.inst.rs1 != -1) {
          const int dep = reg.next_reg[cur.inst.rs1].dep_;
          if (dep == -1) lsb_node.vj = reg.next_reg[cur.inst.rs1].val_;
          else {
            if (next_rob[dep].status == kWrite)lsb_node.vj = next_rob[dep].value;
            else lsb_node.qj = dep;
          }
        }
        //处理rs2
        if (cur.inst.rs2 != -1) {
          const int dep = reg.next_reg[cur.inst.rs2].dep_;
          if (dep == -1) lsb_node.vk = reg.next_reg[cur.inst.rs2].val_;
          else {
            if (next_rob[dep].status == kWrite)lsb_node.vk = next_rob[dep].value;
            else lsb_node.qk = dep;
          }
        }
        //处理立即数
        lsb_node.a = cur.inst.imm;
        //修改rob
        next_rob[index].status = kExec;
        //reg记录依赖
        if (cur.dest != -1) reg.next_reg[cur.dest].dep_ = index;
        //放入entry
        lsb_node.rob_dest = index;
      }
      default: {
        //RS
        if (rs.next_rs.full())break;
        RSNode rs_node;
        rs_node.inst = cur.inst;
        //处理rs1
        if (cur.inst.rs1 != -1) {
          const int dep = reg.next_reg[cur.inst.rs1].dep_;
          if (dep == -1) rs_node.vj = reg.next_reg[cur.inst.rs1].val_;
          else {
            if (next_rob[dep].status == kWrite)rs_node.vj = next_rob[dep].value;
            else rs_node.qj = dep;
          }
        }
        if (cur.inst.op == kAuipc || cur.inst.op == kJal) {
          rs_node.vj = cur.inst.pc; //特殊处理，将PC作为rs1值处理
        }
        //处理rs2
        if (cur.inst.rs2 != -1) {
          const int dep = reg.next_reg[cur.inst.rs2].dep_;
          if (dep == -1) rs_node.vk = reg.next_reg[cur.inst.rs2].val_;
          else {
            if (next_rob[dep].status == kWrite)rs_node.vk = next_rob[dep].value;
            else rs_node.qk = dep;
          }
        }
        //处理立即数
        rs_node.a = cur.inst.imm;
        //修改rob
        next_rob[index].status = kExec;
        //reg记录依赖
        if (cur.dest != -1) reg.next_reg[cur.dest].dep_ = index;
        //放入entry
        rs_node.rob_dest = index;
      }
    }
  }

  void ReorderBuffer::write(const int index) {
    auto cur = now_rob[index];
    cur.status = kCommit;
    if (cur.dest == -1) return;
    //减除RS里指令依赖
    for (int i = 0; i < kRSSize; i++) {
      if (!rs.next_rs.exist(i))continue;
      if (rs.next_rs[i].qj == index) {
        rs.next_rs[i].qj = -1;
        rs.next_rs[i].vj = cur.value;
      }
      if (rs.next_rs[i].qk == index) {
        rs.next_rs[i].qk = -1;
        rs.next_rs[i].vk = cur.value;
      }
    }
    //减除LSB里指令依赖
    if (!lsb.next_lsb.empty()) {
      int i = lsb.next_lsb.head();
      for (int cnt = 0; cnt < lsb.next_lsb.size(); cnt++) {
        if (lsb.next_lsb[i].qj == index) {
          lsb.next_lsb[i].qj = -1;
          lsb.next_lsb[i].vj = cur.value;
        }
        if (lsb.next_lsb[i].qk == index) {
          lsb.next_lsb[i].qk = -1;
          lsb.next_lsb[i].vk = cur.value;
        }
        i = (i + 1) % kLSBSize;
      }
    }
  }

  void ReorderBuffer::commit() {
    const auto cur = now_rob.front();
    switch (cur.inst.op) {
      case kHalt:
        std::cerr << "HALT commit" << std::endl;
        halt = true;
        return;
      case kJal:
        exec_pc = cur.pos;
        if (cur.dest != -1) {
          reg.next_reg[cur.dest].dep_ = -1;
          reg.next_reg[cur.dest].val_ = cur.value;
        }
        break;
      case kJalr:
        exec_pc = next_pc = cur.pos;
        decoder.unfreeze();
        if (cur.dest != -1) {
          reg.next_reg[cur.dest].dep_ = -1;
          reg.next_reg[cur.dest].val_ = cur.value;
        }
        break;
      case kBeq:
      case kBge:
      case kBgeu:
      case kBlt:
      case kBltu:
      case kBne:
        if (cur.inst.predict != static_cast<bool>(cur.value)) {
          //清空，回退状态
          if (cur.value) {
            exec_pc = next_pc = cur.inst.pc + cur.inst.imm;
          } else {
            exec_pc = next_pc = cur.inst.pc + 4;
          }
          decoder.clear();
          reg.clear();
          rs.clear();
          lsb.clear();
          this->clear();
        }
        return;
      case kSb:
        exec_pc = cur.inst.pc + 4;
        memory.store(cur.value, cur.pos, 1);
        lsb.unfreeze();
        break;
      case kSh:
        exec_pc = cur.inst.pc + 4;
        memory.store(cur.value, cur.pos, 2);
        lsb.unfreeze();
        break;
      case kSw:
        exec_pc = cur.inst.pc + 4;
        memory.store(cur.value, cur.pos, 4);
        lsb.unfreeze();
        break;
      default:
        //非跳转指令
        if (cur.dest != -1) {
          reg.next_reg[cur.dest].dep_ = -1;
          reg.next_reg[cur.dest].val_ = cur.value;
        }
        break;
    }
    now_rob.pop();
  }

  void ReorderBuffer::run() {
    if (now_rob.empty())return;
    //提交队首指令
    if (now_rob.front().status == kCommit)commit();
    if (now_rob.empty())return;
    auto i = now_rob.head();
    for (int cnt = 0; cnt < now_rob.size(); cnt++) {
      switch (now_rob[i].status) {
        case kIssue:
          issue(i);
          break;
        case kWrite:
          write(i);
          break;
        default:
          break;
      }
      i = (i + 1) % kROBSize;
    }
  }

  ReorderBuffer rob;
}
