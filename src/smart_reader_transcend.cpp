#include "smart_reader_transcend.hpp"

bool SmartReaderTranscend::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x110005f9);
  if (data_in[0] == 0x54 && data_in[1] == 0x72) {
    printf("Device Model: Transcend\n");
    printf("Secured mode: %d\n", (int)(data_in[11]));
    switch (data_in[16]) {
    case 0x00:
      printf("Bus width: 1 bit\n");
      break;
    case 0x10:
      printf("Bus width: 4 bits\n");
      break;
    }
    switch (data_in[18]) {
    case 0x00:
      printf("Speed mode: Class 0\n");
      break;
    case 0x01:
      printf("Speed mode: Class 2\n");
      break;
    case 0x02:
      printf("Speed mode: Class 4\n");
      break;
    case 0x03:
      printf("Speed mode: Class 6\n");
      break;
    case 0x04:
      printf("Speed mode: Class 10\n");
      break;
    }
    switch (data_in[19]) {
    case 0x00:
      printf("UHS speed grade: Less than 10MB/s\n");
      break;
    case 0x01:
      printf("UHS speed grade: 10MB/s and higher\n");
      break;
    case 0x03:
      printf("UHS speed grade: 30MB/s and higher\n");
      break;
    }
    printf("New bad blocks count: 0x%02x\n", data_in[26]);
    printf("Runtime spare blocks count: 0x%02x\n", data_in[27]);
    printf("Abnormal power drop count: %ld\n",
           (long)((data_in[31] << 24) + (data_in[30] << 16) +
                  (data_in[29] << 8) + data_in[28]));
    printf("Minimum erase count: %ld\n",
           (long)((data_in[35] << 24) + (data_in[34] << 16) +
                  (data_in[33] << 8) + data_in[32]));
    printf("Maximum erase count: %ld\n",
           (long)((data_in[39] << 24) + (data_in[38] << 16) +
                  (data_in[37] << 8) + data_in[36]));
    printf("Total erase count: %ld\n",
           (long)((data_in[43]) + (data_in[42]) + (data_in[41]) + data_in[40]));
    printf("Average erase count: %ld\n",
           (long)((data_in[47] << 24) + (data_in[46] << 16) +
                  (data_in[45] << 8) + data_in[44]));

    printf("Remaining card life: %d%%\n", (int)(data_in[70]));
    printf("Total write CRC count: %d\n",
           mmc_utils.bytes_to_int(data_in[72], data_in[73], data_in[74],
                                  data_in[75]));
    printf("Power On/Off count: %d\n",
           mmc_utils.bytes_to_int(0, 0, data_in[76], data_in[77]));

    printf("NAND flash ID: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",
           data_in[80], data_in[81], data_in[82], data_in[83], data_in[84],
           data_in[85]);
    printf("IC version: %c%c%c%c%c%c%c%c\n", data_in[87], data_in[88], data_in[89],
           data_in[90], data_in[91], data_in[92], data_in[93], data_in[94]);
    printf("fw version: %c%c%c%c%c%c\n", data_in[128], data_in[129],
           data_in[130], data_in[131], data_in[132], data_in[133]);
    printf("Current SD Card Speed Mode: %d\n", (int)data_in[0x6F]);

    return true;
  }
  return false;
}