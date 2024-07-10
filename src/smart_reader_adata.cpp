#include "smart_reader_adata.hpp"

bool SmartReaderAdata::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x110005f1);
  if (data_in[0] == 0x09 && data_in[1] == 0x41) {
    printf("Device Model: Adata\n");
    printf("Factory bad block cnt: %d\n",
           (int)((data_in[24] << 8) + data_in[25]));
    printf("Grown bad block cnt: %d\n", (int)(data_in[26]));
    printf("Spare SLC block cnt: %d\n", (int)(data_in[27]));
    printf("Spare block cnt: %d\n", (int)((data_in[30] << 8) + data_in[31]));
    printf("Data area minimum erase cnt: %ld\n",
           (long)((data_in[32] << 24) + (data_in[33] << 16) +
                  (data_in[34] << 8) + data_in[35]));
    printf("Data area maximum erase cnt: %ld\n",
           (long)((data_in[36] << 24) + (data_in[37] << 16) +
                  (data_in[38] << 8) + data_in[39]));
    printf("Data area total erase cnt: %ld\n",
           (long)((data_in[40] << 24) + (data_in[41] << 16) +
                  (data_in[42] << 8) + data_in[43]));
    printf("Data area average erase cnt: %ld\n",
           (long)((data_in[44] << 24) + (data_in[45] << 16) +
                  (data_in[46] << 8) + data_in[47]));
    printf("System area minimum erase cnt: %ld\n",
           (long)((data_in[48] << 24) + (data_in[49] << 16) +
                  (data_in[50] << 8) + data_in[51]));
    printf("System area maximum erase cnt: %ld\n",
           (long)((data_in[52] << 24) + (data_in[53] << 16) +
                  (data_in[54] << 8) + data_in[55]));
    printf("System area total erase count: %ld\n",
           (long)((data_in[56] << 24) + (data_in[57] << 16) +
                  (data_in[58] << 8) + data_in[59]));
    printf("System area average erase cnt: %ld\n",
           (long)((data_in[60] << 24) + (data_in[61] << 16) +
                  (data_in[62] << 8) + data_in[63]));
    printf("Raw card capacity: %ld MB\n",
           (long)((data_in[64] << 24) + (data_in[65] << 16) +
                  (data_in[66] << 8) + data_in[67]));
    printf("PE Cycle life: %ld\n", (long)((data_in[68] << 8) + data_in[69]));
    printf("Remaining life: %d%%\n", (int)data_in[70]);
    printf("Power cucle cnt: %ld\n",
           (long)((data_in[76] << 24) + (data_in[77] << 16) +
                  (data_in[78] << 8) + data_in[79]));
    printf("Flash ID: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",
           data_in[80], data_in[81], data_in[82], data_in[83], data_in[84],
           data_in[85], data_in[86]);
    printf("Controller: %c%c%c%c%c%c\n", (char)data_in[88], (char)data_in[89],
           (char)data_in[90], (char)data_in[91], (char)data_in[92],
           (char)data_in[93]);
    printf("TLC read reclaim: %ld\n", (long)((data_in[96] << 8) + data_in[97]));
    printf("SLC read reclaim: %ld\n", (long)((data_in[98] << 8) + data_in[99]));
    printf("Firmware block refresh: %ld\n",
           (long)((data_in[100] << 8) + data_in[101]));
    printf("TLC read threshold: %ld\n",
           (long)((data_in[104] << 24) + (data_in[105] << 16) +
                  (data_in[106] << 8) + data_in[107]));
    printf("SLC read threshold: %ld\n",
           (long)((data_in[108] << 24) + (data_in[109] << 16) +
                  (data_in[110] << 8) + data_in[111]));
    printf("FW version: %c%c%c%c%c%c\n", (char)data_in[128], (char)data_in[129],
           (char)data_in[130], (char)data_in[131], (char)data_in[132],
           (char)data_in[133]);
    printf("TLC refresh cnt: %d\n",
           (int)((data_in[136] << 24) + (data_in[137] << 16) +
                 (data_in[138] << 8) + data_in[139]));
    printf("SLC refresh cnt: %d\n",
           (int)((data_in[140] << 24) + (data_in[141] << 16) +
                 (data_in[143] << 8) + data_in[144]));
    return true;
  }
  return false;
}