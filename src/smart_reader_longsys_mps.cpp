#include "smart_reader_longsys_mps.hpp"

bool SmartReaderLongsysMPS::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x1);
  if (data_in[0] == 0x70 && data_in[1] == 0x58) {
    printf("Device model: FORESEE Industrial MPS\n");
    printf("Version: %d\n", mmc_utils.nword_to_int(data_in, 4, 4));
    printf("Size of SMART: %d\n", mmc_utils.nword_to_int(data_in, 12, 4));
    printf("Initial Bad Block Count: %d\n",
           mmc_utils.nword_to_int(data_in, 16, 4));
    printf("Increase Bad Block Count: %d\n",
           mmc_utils.nword_to_int(data_in, 20, 4));
    auto KB_written = mmc_utils.nword_to_int64(data_in, 24) / 2;
    printf("Amount of Data Written: ");
    if (KB_written < 1024) {
      printf("%ld KB \n", KB_written);
    } else if (KB_written < 1024 * 1024) {
      printf("%lf MB \n", (double)KB_written / 1024);
    } else {
      printf("%lf GB \n", (double)KB_written / 1024 / 1024);
    }
    printf("Number of Reserved Blocks: %d\n",
           mmc_utils.nword_to_int(data_in, 32, 4));
    printf("Total Erase Count:: %d\n", mmc_utils.nword_to_int(data_in, 36, 4));
    printf("Total Sector Count: %d\n", mmc_utils.nword_to_int(data_in, 40, 4));
    printf("Remaining Lifetime: %d%%\n",
           mmc_utils.nword_to_int(data_in, 44, 4));
    printf("Remaining data write amount: %d GB\n",
           mmc_utils.nword_to_int(data_in, 48, 4));
    printf("Flash Endurance: %d P/E\n", mmc_utils.nword_to_int(data_in, 52, 4));
    printf("Amount of Data Written to Flash: %d GB\n",
           mmc_utils.nword_to_int(data_in, 56, 4));

    return true;
  } else {
    return false;
  }
}