#include "smart_reader_micron.hpp"

bool SmartReaderMicron::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x110005fb);
    if (data_in[0] == 0x4d && data_in[1] == 0x45) {
      printf("Device model: Micron\n");
      printf("Percentage of life used: %d%%\n", (int)(data_in[7]));
      printf("TLC/QLC area utilization: %d%%\n", (int)(data_in[8]));
      printf("SLC area utilization: %d%%\n", (int)(data_in[9]));
      return true;
    }
    return false;
}