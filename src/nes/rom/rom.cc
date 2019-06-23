#include "nes/rom/rom.hh"

#include <cstdint>
#include <fstream>
#include <iostream>

#include "utils/bit.hh"

nes::rom::ROM::ROM(const std::string& rom_path) { Open(rom_path); }

void nes::rom::ROM::Open(const std::string& rom_path) {
  auto ifs = std::ifstream(rom_path, std::ios::binary);

  auto header = std::vector<uint8_t>(headerSize);
  ifs.read(reinterpret_cast<char*>(&header[0]), header.size());

  for (auto byte : header) {
    std::cout << std::hex << (int)byte << std::endl;
  }

  ParseHeader(header);

  if (header_.trainer) {
    trainer_ = std::vector<uint8_t>(trainerSize);
    ifs.read(reinterpret_cast<char*>(&trainer_[0]), trainer_.size());
  }

  prg_rom_ = std::vector<uint8_t>(header_.prg_rom_size);
  ifs.read(reinterpret_cast<char*>(&prg_rom_[0]), prg_rom_.size());

  chr_rom_ = std::vector<uint8_t>(header_.chr_rom_size);
  ifs.read(reinterpret_cast<char*>(&chr_rom_[0]), chr_rom_.size());
}

void nes::rom::ROM::ParseHeader(const std::vector<uint8_t>& header) {
  header_.prg_rom_size = header[4] * prgROMBase;
  header_.prg_ram_size = header[8] * prgRAMBase;
  header_.chr_rom_size = header[5] * chrROMBase;

  auto flag6 = header[6];
  header_.mirroring = utils::get_bit(flag6, 0);
  header_.prg_ram = utils::get_bit(flag6, 1);
  header_.trainer = utils::get_bit(flag6, 2);
  header_.ignore_mirror = utils::get_bit(flag6, 3);
  header_.mapper = utils::get_range(flag6, 4, 4) << 4;

  auto flag7 = header[7];
  header_.vs_unisys = utils::get_bit(flag7, 0);
  header_.playchoice10 = utils::get_bit(flag7, 1);
  header_.ines2 = utils::get_range(flag7, 2, 2) == 2;
  header_.mapper |= utils::get_range(flag7, 4, 4);

  auto flag9 = header[9];
  header_.tv_sys = utils::get_bit(flag9, 0);
}