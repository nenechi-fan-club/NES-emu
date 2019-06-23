#pragma once

#include <array>
#include <cstdint>
#include <sstream>
#include <vector>

#include "utils/address.hh"

namespace utils {

class Disassembler6502 {
 private:
  using ProgramCounter = std::vector<uint8_t>::const_iterator;
  using ROM = std::vector<uint8_t>;

 private:
  enum class AddressMode {
    ACCUMULATOR = 0,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    IMMEDIATE,
    IMPLIED,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    RELATIVE,
    ZEROPAGE,
    ZEROPAGE_X,
    ZEROPAGE_Y,
  };
  enum class CycleException {
    NOEXCEPT = 0,
    CROSS_PAGE,
    BRANCH,
  };

  struct Instruction {
    uint8_t op_code;
    const char* mnemonic;
    AddressMode addressing;
    uint8_t cycles;
    CycleException exception;
    //Doesn't compile with clang and g++
    //CycleException exception = CycleException::NOEXCEPT;

    Instruction(uint8_t op_code, const char* mnemonic, AddressMode addressing,
                uint8_t cycles)
        : op_code(op_code),
          mnemonic(mnemonic),
          addressing(addressing),
          cycles(cycles),
          exception(CycleException::NOEXCEPT) {}
    Instruction(uint8_t op_code, const char* mnemonic, AddressMode addressing,
                uint8_t cycles, CycleException exception)
        : op_code(op_code),
          mnemonic(mnemonic),
          addressing(addressing),
          cycles(cycles),
          exception(exception) {}
  };

 public:
  inline static const std::array<Instruction, 151> instructionSet = {{
      // clang-format off
      {0x69, "ADC", AddressMode::IMMEDIATE,  2},
      {0x65, "ADC", AddressMode::ZEROPAGE,   3},
      {0x75, "ADC", AddressMode::ZEROPAGE_X, 4},
      {0x6d, "ADC", AddressMode::ABSOLUTE,   4},
      {0x7d, "ADC", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0x79, "ADC", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0x61, "ADC", AddressMode::INDIRECT_X, 6},
      {0x71, "ADC", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0x29, "AND", AddressMode::IMMEDIATE,  2},
      {0x25, "AND", AddressMode::ZEROPAGE,   3},
      {0x35, "AND", AddressMode::ZEROPAGE_X, 4},
      {0x2d, "AND", AddressMode::ABSOLUTE,   4},
      {0x3d, "AND", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0x39, "AND", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0x21, "AND", AddressMode::INDIRECT_X, 6},
      {0x31, "AND", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0x0a, "ASL", AddressMode::ACCUMULATOR, 2},
      {0x06, "ASL", AddressMode::ZEROPAGE,    5},
      {0x16, "ASL", AddressMode::ZEROPAGE_X,  6},
      {0x0e, "ASL", AddressMode::ABSOLUTE,    6},
      {0x1e, "ASL", AddressMode::ABSOLUTE_X,  7},

      {0x90, "BCC", AddressMode::RELATIVE, 2, CycleException::BRANCH},
      {0xb0, "BCS", AddressMode::RELATIVE, 2, CycleException::BRANCH},
      {0xf0, "BEQ", AddressMode::RELATIVE, 2, CycleException::BRANCH},

      {0x24, "BIT", AddressMode::ZEROPAGE, 3},
      {0x24, "BIT", AddressMode::ABSOLUTE, 4},

      {0x30, "BMI", AddressMode::RELATIVE, 2, CycleException::BRANCH},
      {0xd0, "BNE", AddressMode::RELATIVE, 2, CycleException::BRANCH},
      {0x10, "BPL", AddressMode::RELATIVE, 2, CycleException::BRANCH},
      {0x00, "BRK", AddressMode::IMPLIED, 7},
      {0x50, "BVC", AddressMode::RELATIVE, 2, CycleException::BRANCH},
      {0x70, "BVS", AddressMode::RELATIVE, 2, CycleException::BRANCH},

      {0x18, "CLC", AddressMode::IMPLIED, 2},
      {0xd8, "CLD", AddressMode::IMPLIED, 2},
      {0x58, "CLI", AddressMode::IMPLIED, 2},
      {0xb8, "CLV", AddressMode::IMPLIED, 2},

      {0xc9, "CMP", AddressMode::IMMEDIATE,  2},
      {0xc5, "CMP", AddressMode::ZEROPAGE,   3},
      {0xd9, "CMP", AddressMode::ZEROPAGE_X, 4},
      {0xcd, "CMP", AddressMode::ABSOLUTE,   4},
      {0xdd, "CMP", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0xd9, "CMP", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0xc1, "CMP", AddressMode::INDIRECT_X, 6},
      {0xd1, "CMP", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0xe0, "CPX", AddressMode::IMMEDIATE, 2},
      {0xe4, "CPX", AddressMode::ZEROPAGE,  3},
      {0xec, "CPX", AddressMode::ABSOLUTE,  4},

      {0xc0, "CPY", AddressMode::IMMEDIATE, 2},
      {0xc4, "CPY", AddressMode::ZEROPAGE,  3},
      {0xcc, "CPY", AddressMode::ABSOLUTE,  4},

      {0xc6, "DEC", AddressMode::ZEROPAGE,   5},
      {0xd6, "DEC", AddressMode::ZEROPAGE_X, 6},
      {0xce, "DEC", AddressMode::ABSOLUTE,   3},
      {0xde, "DEC", AddressMode::ABSOLUTE_X, 7},

      {0xca, "DEX", AddressMode::IMPLIED, 2},
      {0x88, "DEY", AddressMode::IMPLIED, 2},

      {0x49, "EOR", AddressMode::IMMEDIATE,  2},
      {0x45, "EOR", AddressMode::ZEROPAGE,   3},
      {0x55, "EOR", AddressMode::ZEROPAGE_X, 4},
      {0x4d, "EOR", AddressMode::ABSOLUTE,   4},
      {0x5d, "EOR", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0x59, "EOR", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0x41, "EOR", AddressMode::INDIRECT_X, 6},
      {0x51, "EOR", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0xe6, "INC", AddressMode::ZEROPAGE,    5},
      {0xf6, "INC", AddressMode::ZEROPAGE_X,  6},
      {0xee, "INC", AddressMode::ABSOLUTE,    6},
      {0xfe, "INC", AddressMode::ABSOLUTE_X,  7},

      {0xe8, "INX", AddressMode::IMPLIED, 2},
      {0xc8, "INY", AddressMode::IMPLIED, 2},

      {0x4c, "JMP", AddressMode::ABSOLUTE, 3},
      {0x6c, "JMP", AddressMode::INDIRECT, 5},

      {0x20, "JSR", AddressMode::ABSOLUTE, 6},

      {0xa9, "LDA", AddressMode::IMMEDIATE,  2},
      {0xa5, "LDA", AddressMode::ZEROPAGE,   3},
      {0xb5, "LDA", AddressMode::ZEROPAGE_X, 4},
      {0xad, "LDA", AddressMode::ABSOLUTE,   4},
      {0xbd, "LDA", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0xb9, "LDA", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0xa1, "LDA", AddressMode::INDIRECT_X, 6},
      {0xb1, "LDA", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0xa2, "LDX", AddressMode::IMMEDIATE,  2},
      {0xa6, "LDX", AddressMode::ZEROPAGE,   3},
      {0xb6, "LDX", AddressMode::ZEROPAGE_Y, 4},
      {0xae, "LDX", AddressMode::ABSOLUTE,   4},
      {0xbe, "LDX", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},

      {0xa0, "LDY", AddressMode::IMMEDIATE,  2},
      {0xa4, "LDY", AddressMode::ZEROPAGE,   3},
      {0xb4, "LDY", AddressMode::ZEROPAGE_X, 4},
      {0xac, "LDY", AddressMode::ABSOLUTE,   4},
      {0xbc, "LDY", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},

      {0x4a, "LSR", AddressMode::IMMEDIATE,  2},
      {0x46, "LSR", AddressMode::ZEROPAGE,   5},
      {0x56, "LSR", AddressMode::ZEROPAGE_X, 6},
      {0x4e, "LSR", AddressMode::ABSOLUTE,   6},
      {0x5e, "LSR", AddressMode::ABSOLUTE_X, 7},

      {0xea, "NOP", AddressMode::IMPLIED, 2},

      {0x09, "ORA", AddressMode::IMMEDIATE,  2},
      {0x05, "ORA", AddressMode::ZEROPAGE,   3},
      {0x15, "ORA", AddressMode::ZEROPAGE_X, 4},
      {0x0d, "ORA", AddressMode::ABSOLUTE,   4},
      {0x1d, "ORA", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0x19, "ORA", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0x01, "ORA", AddressMode::INDIRECT_X, 6},
      {0x11, "ORA", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0x48, "PHA", AddressMode::IMPLIED, 3},
      {0x08, "PHP", AddressMode::IMPLIED, 3},
      {0x68, "PLA", AddressMode::IMPLIED, 4},
      {0x28, "PLP", AddressMode::IMPLIED, 4},

      {0x2a, "ROL", AddressMode::IMMEDIATE,  2},
      {0x26, "ROL", AddressMode::ZEROPAGE,   5},
      {0x36, "ROL", AddressMode::ZEROPAGE_X, 6},
      {0x2e, "ROL", AddressMode::ABSOLUTE,   6},
      {0x3e, "ROL", AddressMode::ABSOLUTE_X, 7},

      {0x6a, "ROR", AddressMode::IMMEDIATE,  2},
      {0x66, "ROR", AddressMode::ZEROPAGE,   5},
      {0x76, "ROR", AddressMode::ZEROPAGE_X, 6},
      {0x6e, "ROR", AddressMode::ABSOLUTE,   6},
      {0x7e, "ROR", AddressMode::ABSOLUTE_X, 7},

      {0x40, "RTI", AddressMode::IMPLIED, 6},
      {0x60, "RTS", AddressMode::IMPLIED, 6},

      {0xe9, "SBC", AddressMode::IMMEDIATE,  2},
      {0xe5, "SBC", AddressMode::ZEROPAGE,   3},
      {0xf5, "SBC", AddressMode::ZEROPAGE_X, 4},
      {0xed, "SBC", AddressMode::ABSOLUTE,   4},
      {0xfd, "SBC", AddressMode::ABSOLUTE_X, 4, CycleException::CROSS_PAGE},
      {0xf9, "SBC", AddressMode::ABSOLUTE_Y, 4, CycleException::CROSS_PAGE},
      {0xe1, "SBC", AddressMode::INDIRECT_X, 6},
      {0xf1, "SBC", AddressMode::INDIRECT_Y, 5, CycleException::CROSS_PAGE},

      {0x38, "SEC", AddressMode::IMPLIED, 2},
      {0xf8, "SED", AddressMode::IMPLIED, 2},
      {0x78, "SEI", AddressMode::IMPLIED, 2},

      {0x85, "STA", AddressMode::ZEROPAGE,   3},
      {0x99, "STA", AddressMode::ZEROPAGE_X, 4},
      {0x8d, "STA", AddressMode::ABSOLUTE,   4},
      {0x9d, "STA", AddressMode::ABSOLUTE_X, 5},
      {0x99, "STA", AddressMode::ABSOLUTE_Y, 5},
      {0x81, "STA", AddressMode::INDIRECT_X, 6},
      {0x91, "STA", AddressMode::INDIRECT_Y, 6},

      {0x86, "STX", AddressMode::ZEROPAGE,   3},
      {0x96, "STX", AddressMode::ZEROPAGE_Y, 4},
      {0x8e, "STX", AddressMode::ABSOLUTE,   4},

      {0x84, "STY", AddressMode::ZEROPAGE,   3},
      {0x94, "STY", AddressMode::ZEROPAGE_X, 4},
      {0x8c, "STY", AddressMode::ABSOLUTE,   4},

      {0xaa, "TAX", AddressMode::IMPLIED, 2},
      {0xa8, "TAY", AddressMode::IMPLIED, 2},
      {0xba, "TSX", AddressMode::IMPLIED, 2},
      {0x8a, "TXA", AddressMode::IMPLIED, 2},
      {0x9a, "TXS", AddressMode::IMPLIED, 2},
      {0x98, "TYA", AddressMode::IMPLIED, 2},
      // clang-format on
  }};

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
