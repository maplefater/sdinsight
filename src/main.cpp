#include "mmc_utils.hpp"
#include "smart_reader.hpp"
#include <iostream>
#include <unistd.h>
int main(int argc, char *argv[]) {
  auto uid = getuid();
  if (uid != 0) {
    std::cout << "root required." << std::endl;
    return -1;
  }
  std::string device = "/dev/mmcblk0";
  if (argc < 2) {
    std::cout << "no device provided, use default: " << device << std::endl
              << std::endl;
  } else {
    device = argv[1];
  }
  MMCutils mmc_utils(device);
  readSmart(mmc_utils);
  return 0;
}