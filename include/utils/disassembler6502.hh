#pragma once

#include <array>
#include <cstdint>
#include <sstream>
#include <vector>

#include "nes/rom/rom.hh"
#include "utils/address.hh"

namespace utils {

class Disassembler6502 {
 private:
  using ProgramCounter = std::vector<uint8_t>::const_iterator;

 private:
  ProgramCounter pc_;
  nes::rom::ROM rom_;

  std::stringstream disassembly_;

 public:
  Disassembler6502() = default;
  Disassembler6502(const nes::rom::ROM& rom);
  Disassembler6502(nes::rom::ROM&& rom);
  Disassembler6502(const std::string& path_to_rom);
  Disassembler6502(const Disassembler6502& disassembler);
  Disassembler6502(Disassembler6502&& disassembler) noexcept;

 public:
  void Open(const nes::rom::ROM& rom);
  void Open(nes::rom::ROM&& rom);
  void Open(const std::string& path_to_rom);

  inline bool eof() const {
    return rom_.prg_rom().empty() || pc_ == rom_.prg_rom().end() - 6;
  }
  inline std::string get_disassembly() const { return disassembly_.str(); }
  inline uint16_t get_pc() const {
    return (uint16_t)std::distance(rom_.prg_rom().begin(), pc_);
  }

  void operator()();
};

}  // namespace utils
