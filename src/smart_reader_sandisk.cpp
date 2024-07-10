#include "smart_reader_sandisk.hpp"

bool SmartReaderSandisk::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x1);
  if (data_in[0] == 0x44 && (data_in[1] == 0x53 || data_in[1] == 0x57)) {
    printf("Device model: Sandisk\n");
  } else {
    return false;
  }
  char tmpstr[100];
  /*
        Health Status is an estimated percent life used based on the amount of
     TBW the NAND memory has experienced relative to the SD card device TBW
     ability. Values reported in hexadecimal in 1% increments with 0x01
     representing 0.0% to 0.99% used. A value of 0x64 indicates 99 to 99.99% of
     the ability have been used. The SD card storage device may accommodate
     writes in excess of the 100% expected life limit. Note that although this
     is possible, entry into a read only mode could occur upon the next write
     cycle.
  */

  strncpy(tmpstr, (char *)&data_in[2], 6);
  tmpstr[6] = 0;
  printf("Manufacture date(YYMMDD): %s\n", tmpstr);
  printf("Health Status Percent Used: %d\n", data_in[8]);
  printf("Feature Revision: 0x%x\n", data_in[11]);
  printf("Generation Identifier: %d\n", data_in[14]);
  strncpy(tmpstr, (char *)&data_in[49], 32);
  tmpstr[32] = 0;
  printf("Programmable Product String: %s\n", tmpstr);
  printf("Power-on times: %d\n",
         mmc_utils.bytes_to_int(0, data_in[24], data_in[25], data_in[26]));
  data_in[417] = 0x20;
  strncpy(tmpstr, (char *)&data_in[405], 20);
  tmpstr[20] = 0;
  printf("Product number: %s\n", tmpstr);
  printf("SN: %02X %02X %02X %02X %02X %02X\n", data_in[426], data_in[427],
         data_in[428], data_in[429], data_in[430], data_in[431]);
  return true;
}