#include "mmc_utils.hpp"
#include "smart_reader.hpp"
#include "smi_flash_id.hpp"
#include <iostream>
#include <unistd.h>
int main() {
  auto uid = getuid();
  if (uid != 0) {
    std::cout << "root required." << std::endl;
    return -1;
  }
  MMCutils mmc_utils{};
  std::cout << "====================CID info====================" << std::endl;
  mmc_utils.printCid();
  // std::cout << "====================EXT CSD info====================" << std::endl;
  // mmc_utils.printExtCsd();
  std::cout << "====================smi flash id====================" << std::endl;
  printSmiFlashId();
  std::cout << "====================SMART info====================" << std::endl;
  readSmart(mmc_utils);
  return 0;
}
