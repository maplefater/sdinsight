#include "smart_reader_atp.hpp"
bool SmartReaderATP::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x11000001);
  static char SpeedClass[5] = {0, 2, 4, 6, 10};
  static char UHSSpeed[4][9] = {"<10MB/s", ">10MB/s", "Reserved", ">30MB/s"};
  uint64_t Conversion = 0;
  // PE cycle > 0, SM2707
  if (data_in[52] > 0 || data_in[53] > 0) {
    printf("Device model: ATP\n");
    printf("Controller: SM2707\n");
    printf("Speed Class: %d\n", SpeedClass[data_in[2]]);
    printf("UHS Speed Grade: %s\n", UHSSpeed[data_in[3]]);
    Conversion = (data_in[8] << 8) + (data_in[9]);
    printf("Original Bad Block Count: %lu\n", Conversion);
    printf("Run-time Bad Block Count: %d\n", data_in[10]);
    printf("Spare Block Count: %d\n", data_in[11]);
    Conversion = (data_in[16] << 24) + (data_in[17] << 16) +
                 (data_in[18] << 8) + (data_in[19]);
    printf("Min. Erase Count MLC_TLC: %lu\n", Conversion);
    Conversion = (data_in[20] << 24) + (data_in[21] << 16) +
                 (data_in[22] << 8) + (data_in[23]);
    printf("Max. Erase Count MLC_TLC: %lu\n", Conversion);
    Conversion = (data_in[24] << 24) + (data_in[25] << 16) +
                 (data_in[26] << 8) + (data_in[27]);
    printf("Total Erase Count MLC_TLC: %lu\n", Conversion);
    Conversion = (data_in[28] << 24) + (data_in[29] << 16) +
                 (data_in[30] << 8) + (data_in[31]);
    printf("Avg. Erase Count MLC_TLC: %lu\n", Conversion);
    Conversion = (data_in[32] << 24) + (data_in[33] << 16) +
                 (data_in[34] << 8) + (data_in[35]);
    printf("Min. Erase Count SLC: %lu\n", Conversion);
    Conversion = (data_in[36] << 24) + (data_in[37] << 16) +
                 (data_in[38] << 8) + (data_in[39]);
    printf("Max. Erase Count SLC: %lu\n", Conversion);
    Conversion = (data_in[40] << 24) + (data_in[41] << 16) +
                 (data_in[42] << 8) + (data_in[43]);
    printf("Total Erase Count SLC: %lu\n", Conversion);
    Conversion = (data_in[44] << 24) + (data_in[45] << 16) +
                 (data_in[46] << 8) + (data_in[47]);
    printf("Avg. Erase Count SLC: %lu\n", Conversion);
    Conversion = (data_in[48] << 24) + (data_in[49] << 16) +
                 (data_in[50] << 8) + (data_in[51]);
    printf("Raw Capacity (MB): %lu\n", Conversion);
    Conversion = (data_in[52] << 8) + (data_in[53]);
    printf("NAND P_E Cycles: %lu\n", (100 * Conversion));
    printf("Card Life: %d\n", data_in[54]);
    Conversion = (data_in[60] << 24) + (data_in[61] << 16) +
                 (data_in[62] << 8) + (data_in[63]);
    printf("Power ON_OFF Count: %lu\n", Conversion);
    return true;
  } else if (data_in[2] > 0 || data_in[3] > 0) { // SM2702
    printf("Device model: ATP\n");
    printf("Controller: SM2702\n");
    printf("Bus Width: %d\n", data_in[0] >> 2);
    printf("Speed Class: %d\n", SpeedClass[data_in[2]]);
    printf("UHS Speed Grade: %d\n", data_in[3]);
    Conversion = (data_in[4] << 24) + (data_in[5] << 16) + (data_in[6] << 8) +
                 (data_in[7]);
    printf("Protect. Area Size: %lu\n", Conversion);
    printf("Spare Block Count: %d\n", data_in[8]);
    printf("Later Bad Blok Count: %d\n", data_in[10]);
    Conversion = (data_in[12] << 24) + (data_in[13] << 16) +
                 (data_in[14] << 8) + (data_in[15]);
    printf("Sudden Power OFF Reset count: %lu\n", Conversion);
    Conversion = (data_in[16] << 24) + (data_in[17] << 16) +
                 (data_in[18] << 8) + (data_in[19]);
    printf("Min. Erase Count: %lu\n", Conversion);
    Conversion = (data_in[20] << 24) + (data_in[21] << 16) +
                 (data_in[22] << 8) + (data_in[23]);
    printf("Max. Erase Count: %lu\n", Conversion);
    Conversion = (data_in[24] << 24) + (data_in[25] << 16) +
                 (data_in[26] << 8) + (data_in[27]);
    printf("Total Erase Count: %lu\n", Conversion);
    Conversion = (data_in[28] << 24) + (data_in[29] << 16) +
                 (data_in[30] << 8) + (data_in[31]);
    printf("Avg. Erase Count: %lu\n", Conversion);
    return true;
  } else {
    return false;
  }
}