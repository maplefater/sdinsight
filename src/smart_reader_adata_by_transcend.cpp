#include "smart_reader_adata_by_transcend.hpp"

bool SmartReaderAdataByTranscend::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x110005f9);
  char tmpstr[100];
  if (data_in[176] == 0x54 && data_in[177] == 0x52 && data_in[178] == 0x41 &&
      data_in[179] == 0x4E && data_in[180] == 0x53 && data_in[181] == 0x43 &&
      data_in[182] == 0x45 && data_in[183] == 0x4E && data_in[184] == 0x44) {
    printf("Device Model: Adata By Transcend\n");
    printf("Flash ID: %02X %02X %02X %02X %02X %02x\n", data_in[0], data_in[1],
           data_in[2], data_in[3], data_in[4], data_in[5]);
    printf("IC Version: [0x%02x,0x%02x]\n", data_in[9], data_in[10]);
    printf("FW Version: [%02d,%02d]\n", data_in[11],
           data_in[12]); // show in decimal
    printf("CE Number: %d\n", (int)data_in[14]);
    printf("Total Bad Block Count: %d\n",
           mmc_utils.bytes_to_int(data_in[32], data_in[33], data_in[34],
                                  data_in[35]));
    printf("Spare Block Rate: %f%%\n",
           ((float)((((unsigned int)data_in[64]) << 8) |
                    ((unsigned int)data_in[65]))));
    printf("Total Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[80], data_in[81], data_in[82],
                                  data_in[83]));
    printf("Remain Life: %f%%\n", ((float)((((unsigned int)data_in[96]) << 8) |
                                           ((unsigned int)data_in[97]))));
    printf("Average Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[104], data_in[105], data_in[98],
                                  data_in[99]));
    printf("Minimum Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[106], data_in[107], data_in[100],
                                  data_in[101]));
    printf("Maximum Erase Count: %d\n",
           mmc_utils.bytes_to_int(data_in[108], data_in[109], data_in[102],
                                  data_in[103]));
    printf("Power Up Count: %d\n",
           mmc_utils.bytes_to_int(data_in[112], data_in[113], data_in[114],
                                  data_in[115]));
    printf("Abnormal Power Off Count: %d\n",
           mmc_utils.bytes_to_int(0, 0, data_in[128], data_in[129]));
    strncpy(tmpstr, (char *)&data_in[176], 9);
    tmpstr[9] = 0;
    printf("Product Marker: %s\n", tmpstr);
    return true;
  }
  return false;
}
