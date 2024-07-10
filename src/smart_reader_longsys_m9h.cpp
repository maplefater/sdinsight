#include "smart_reader_longsys_m9h.hpp"

bool SmartReaderLongsysM9H::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x110005FD);
  if (data_in[0] == 0x4C && data_in[1] == 0x58) {
    printf("Device model: longsys M9H\n");
    printf("Spare Block Count: %d\n", (int)data_in[24]);
    printf("Initial Bad Block Count: %d\n", (int)data_in[25]);
    printf("Later Bad Block Count: %d\n", (int)data_in[26]);
    printf("Abnormal Power Off Count: %d\n",
           mmc_utils.bytes_to_int(data_in[28], data_in[29], data_in[30],
                                  data_in[31]));
    printf("Minimum Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[32], data_in[33], data_in[34],
                                  data_in[35]));
    printf("Maximum Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[36], data_in[37], data_in[38],
                                  data_in[39]));
    printf("Total Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[40], data_in[41], data_in[42],
                                  data_in[43]));
    printf("Average Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[44], data_in[45], data_in[46],
                                  data_in[47]));
    char tmpstr[100];
    strncpy(tmpstr, (char *)&data_in[53], 16);
    tmpstr[16] = 0;
    printf("Product Marker: %s\n", tmpstr);
    return true;
  } else {
    return false;
  }
}