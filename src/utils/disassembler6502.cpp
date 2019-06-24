#include "utils/disassembler6502.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "nes/cpu/instructions.hpp"
#include "nes/rom/rom.hpp"
#include "utils/address.hpp"

utils::Disassembler6502::Disassembler6502(const nes::rom::ROM& rom) {
  Open(rom);
}
utils::Disassembler6502::Disassembler6502(nes::rom::ROM&& rom) {
  Open(std::move(rom));
}
utils::Disassembler6502::Disassembler6502(const std::string& path_to_rom) {
  Open(path_to_rom);
}
utils::Disassembler6502::Disassembler6502(const Disassembler6502& disassembler)
    : rom_(disassembler.rom_) {
  pc_ = rom_.prg_rom().begin() + disassembler.get_pc();
  disassembly_ << disassembler.disassembly_.rdbuf();
}
utils::Disassembler6502::Disassembler6502(
    Disassembler6502&& disassembler) noexcept
    : disassembly_(std::move(disassembler.disassembly_)) {
  pc_ = disassembler.rom_.prg_rom().begin() + disassembler.get_pc();
  std::move(disassembler.pc_);
  rom_ = std::move(disassembler.rom_);
}

void utils::Disassembler6502::Open(const nes::rom::ROM& rom) {
  rom_ = rom;
  pc_ = rom_.prg_rom().begin();
  disassembly_.clear();
}
void utils::Disassembler6502::Open(nes::rom::ROM&& rom) {
  rom_ = std::move(rom);
  pc_ = rom_.prg_rom().begin();
  disassembly_.clear();
}
void utils::Disassembler6502::Open(const std::string& path_to_rom) {
  Open(nes::rom::ROM{path_to_rom});
}

void utils::Disassembler6502::operator()() {
  if (eof()) {
    return;
  }

  if (pc_ == rom_.prg_rom().begin()) {
    disassembly_ << std::setfill(' ') << std::setw(22) << "* = 0000"
                 << std::endl;
  }

  auto op_code = *pc_;
  std::stringstream line;
  std::stringstream machine;
  std::stringstream assembly;

  auto it = std::find_if(nes::cpu::instructionSet.begin(),
                         nes::cpu::instructionSet.end(),
                         [op_code](const nes::cpu::Instruction& a) {
                           return a.op_code == op_code;
                         });

  if (it == nes::cpu::instructionSet.end()) {
    // skip NOP when parsing ROMs
    ++pc_;
    return;
  }

  auto instruction = *it;

  line << std::setfill('0') << std::setw(4) << std::hex << get_pc() << " ";

  switch (instruction.addressing) {
    case nes::cpu::AddressMode::ACCUMULATOR:
      machine << std::hex << (int)*pc_;
      assembly << instruction.mnemonic << " A";
      ++pc_;
      break;
    case nes::cpu::AddressMode::ABSOLUTE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1) << " "
              << (int)*(pc_ + 2);
      assembly << instruction.mnemonic << " $" << std::hex
               << LittleEndian(*(pc_ + 1), *(pc_ + 2));
      pc_ += 3;
      break;
    case nes::cpu::AddressMode::ABSOLUTE_X:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1) << " "
              << (int)*(pc_ + 2);
      assembly << instruction.mnemonic << " $" << std::hex
               << LittleEndian(*(pc_ + 1), *(pc_ + 2)) << ",X";
      pc_ += 3;
      break;
    case nes::cpu::AddressMode::ABSOLUTE_Y:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1) << " "
              << (int)*(pc_ + 2);
      assembly << instruction.mnemonic << " $" << std::hex
               << LittleEndian(*(pc_ + 1), *(pc_ + 2)) << ",Y";
      pc_ += 3;
      break;
    case nes::cpu::AddressMode::IMMEDIATE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " #$" << std::hex << (int)*(pc_ + 1);
      pc_ += 2;
      break;
    case nes::cpu::AddressMode::IMPLIED:
      machine << std::hex << (int)*pc_;
      assembly << instruction.mnemonic;
      ++pc_;
      break;
    case nes::cpu::AddressMode::INDIRECT:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " ($" << std::hex << (int)*(pc_ + 1)
               << ")";
      pc_ += 3;
      break;
    case nes::cpu::AddressMode::INDIRECT_X:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " ($" << std::hex << (int)*(pc_ + 1)
               << ",X)";
      pc_ += 2;
      break;
    case nes::cpu::AddressMode::INDIRECT_Y:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " ($" << std::hex << (int)*(pc_ + 1)
               << ",Y)";
      pc_ += 2;
      break;
    case nes::cpu::AddressMode::RELATIVE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1);
      pc_ += 2;
      break;
    case nes::cpu::AddressMode::ZEROPAGE:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1);
      pc_ += 2;
      break;
    case nes::cpu::AddressMode::ZEROPAGE_X:
      machine << std::hex << (int)*pc_ << " " << (int)*(pc_ + 1);
      assembly << instruction.mnemonic << " $" << std::hex << (int)*(pc_ + 1)
               << ",X";
      pc_ += 2;
      break;
    case nes::cpu::AddressMode::ZEROPAGE_Y:
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