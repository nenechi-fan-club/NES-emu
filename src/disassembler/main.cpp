#include <fstream>
#include <iostream>

#include "nes/rom/rom.hpp"
#include "utils/disassembler6502.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    return 1;
  }

  auto path_to_rom = std::string(argv[1]);
  auto path_to_disassembly =
      std::string(argc > 2 ? argv[2] : "disassembly.txt");
  auto dis = utils::Disassembler6502(path_to_rom);

  while (!dis.eof()) {
    dis();
  }

  auto ofs = std::ofstream(path_to_disassembly);
  ofs << dis.get_disassembly();

  return 0;
}