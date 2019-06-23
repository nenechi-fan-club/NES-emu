#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nes {
namespace rom {

// TODO: add NES2.0 support

class ROM {
 public:
  static const uint16_t prgROMBase = 16384;
  static const uint16_t prgRAMBase = 8192;
  static const uint16_t chrROMBase = 8192;
  static const uint16_t headerSize = 16;
  static const uint16_t trainerSize = 512;

 private:
  struct Header {
    bool mirroring;
    bool prg_ram;
    bool trainer;
    bool ignore_mirror;

    bool vs_unisys;
    bool playchoice10;

    uint8_t tv_sys;

    bool ines2;

    uint8_t mapper;

    uint16_t prg_rom_size;
    uint16_t chr_rom_size;
    uint16_t prg_ram_size;

  } header_;

 private:
  std::vector<uint8_t> trainer_;
  std::vector<uint8_t> prg_rom_;
  std::vector<uint8_t> chr_rom_;

 public:
  ROM() = default;
  ROM(const std::string& rom_path);
  ROM(const ROM& rom) = default;
  ROM(ROM&& rom) = default;

 public:
  inline std::vector<uint8_t> prg_rom() const { return prg_rom_; }

 public:
  void Open(const std::string& rom_path);

 private:
  void ParseHeader(const std::vector<uint8_t>& header);
};

}  // namespace rom
}  // namespace nes