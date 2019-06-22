#include "utils/disassembler6502.hh"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "utils/address.hh"

utils::Disassembler6502::Disassembler6502(const ROM& rom) { Open(rom); }
utils::Disassembler6502::Disassembler6502(ROM&& rom) { Open(std::move(rom)); }
utils::Disassembler6502::Disassembler6502(const Disassembler6502& disassembler)
    : rom_(disassembler.rom_) {
  pc_ = rom_.begin() + disassembler.get_pc();
  disassembly_ << disassembler.disassembly_.rdbuf();
}
utils::Disassembler6502::Disassembler6502(
    Disassembler6502&& disassembler) noexcept
    : disassembly_(std::move(disassembler.disassembly_)) {
  pc_ = disassembler.rom_.begin() + disassembler.get_pc();
  std::move(disassembler.pc_);
  rom_ = std::move(disassembler.rom_);
}

void utils::Disassembler6502::Open(const ROM& rom) {
  rom_ = rom;
  pc_ = rom_.begin();
  disassembly_.clear();
}

void utils::Disassembler6502::Open(ROM&& rom) {
  rom_ = std::move(rom);
  pc_ = rom_.begin();
  disassembly_.clear();
}

void utils::Disassembler6502::operator()() {
  if (eof()) {
    return;
  }

  if (pc_ == rom_.begin()) {
    disassembly_ << std::setfill(' ') << std::setw(22) << "* = 0000"
                 << std::endl;
  }

  auto op_code = *pc_;
  std::stringstream line;
  std::stringstream machine;
  std::stringstream assembly;

  auto it = std::find_if(
      instructionSet.begin(), instructionSet.end(),
      [op_code](const Instruction& a) { return a.op_code == op_code; });

  if (it == instructionSet.end()) {
    return;
  }

  auto instruction = *it;

  line << std::setfill('0') << std::setw(4) << std::hex << get_pc() << " ";

  switch (instruction.addressing) {
    case AddressMode::ACCUMULATOR:
      machine << std::hex << (int)*pc_;
      assembly << instruction.mnemonic << " A";
      ++pc_;
      break;
    case AddressMode::ABSOLUTE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1) << " "
              << (int)*(pc_ + 2);
      assembly << instruction.mnemonic << " $" << std::hex
               << LittleEndian(*(pc_ + 1), *(pc_ + 2));
      pc_ += 3;
      break;
    case AddressMode::ABSOLUTE_X:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1) << " "
              << (int)*(pc_ + 2);
      assembly << instruction.mnemonic << " $" << std::hex
               << LittleEndian(*(pc_ + 1), *(pc_ + 2)) << ",X";
      pc_ += 3;
      break;
    case AddressMode::ABSOLUTE_Y:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1) << " "
              << (int)*(pc_ + 2);
      assembly << instruction.mnemonic << " $" << std::hex
               << LittleEndian(*(pc_ + 1), *(pc_ + 2)) << ",Y";
      pc_ += 3;
      break;
    case AddressMode::IMMEDIATE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " #$" << std::hex << (int)*(pc_ + 1);
      pc_ += 2;
      break;
    case AddressMode::IMPLIED:
      machine << std::hex << (int)*pc_;
      assembly << instruction.mnemonic;
      ++pc_;
      break;
    case AddressMode::INDIRECT:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " ($" << std::hex << (int)*(pc_ + 1)
               << ")";
      pc_ += 3;
      break;
    case AddressMode::INDIRECT_X:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " ($" << std::hex << (int)*(pc_ + 1)
               << ",X)";
      pc_ += 2;
      break;
    case AddressMode::INDIRECT_Y:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " ($" << std::hex << (int)*(pc_ + 1)
               << ",Y)";
      pc_ += 2;
      break;
    case AddressMode::RELATIVE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1);
      pc_ += 2;
      break;
    case AddressMode::ZEROPAGE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1);
      pc_ += 2;
      break;
    case AddressMode::ZEROPAGE_X:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1)
               << ",X";
      pc_ += 2;
      break;
    case AddressMode::ZEROPAGE_Y:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1)
               << ",Y";
      pc_ += 2;
      break;
  }

  disassembly_ << line.str() << std::left << std::setfill(' ') << std::setw(9)
               << machine.str() << assembly.str() << std::endl;

  if (eof()) {
    disassembly_ << std::right << std::setfill('0') << std::setw(4) << std::hex
                 << get_pc();
    disassembly_ << std::setfill(' ') << std::setw(14) << ".END" << std::endl;
  }
}