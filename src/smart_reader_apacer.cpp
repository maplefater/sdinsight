#include "smart_reader_apacer.hpp"
#include <unistd.h>

bool SmartReaderApacer::readSmart(MMCutils &mmc_utils) {
  mmc_utils.CMD56_write(0x00000010);
  usleep(1000000);
  auto data_in = mmc_utils.CMD56_read(0x00000021);
  // CE count
  if (data_in[14] > 0 && data_in[14] <= 16) {
    printf("Device model: Apacer\n");
    printf("Flash Id: "
           "[0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,"
           "0x%02x,0x%02x,0x%02x]\n",
           data_in[0], data_in[1], data_in[2], data_in[3], data_in[4],
           data_in[5], data_in[6], data_in[7], data_in[8]);
    printf("IC Version: [0x%02x,0x%02x]\n", data_in[9], data_in[10]);
    printf("FW Version: [%02d,%02d]\n", data_in[11],
           data_in[12]); // show in decimal
    printf("CE Number: %d\n", (int)data_in[14]);

    printf("Bad Block Replace Maximum:: %d\n",
           mmc_utils.bytes_to_int(0, 0, data_in[16], data_in[17]));

    printf("Spare Block Count: %d\n", (int)((data_in[16] << 8) + data_in[17]));

    // sum up to get initial bad block count
    int64_t sum = 0;
    for (int i = 32; i < 64; i++) {
      sum += data_in[i];
    }
    printf("Initial Bad Block Count: %ld\n", sum);

    printf("Good Block Rate: %2.2f%%\n",
           (float)((float)((int)((data_in[64] << 8) + data_in[65])) / 100));

    printf("Total Erase Count: %ld\n",
           (long)((data_in[80] << 24) + (data_in[81] << 16) +
                  (data_in[82] << 8) + data_in[83]));

    printf("Remain Life: %2.2f%%\n",
           (float)((float)((int)((data_in[96] << 8) + data_in[97])) / 100));

    printf("Average Erase Count: %ld\n",
           (long)((data_in[104] << 24) + (data_in[105] << 16) +
                  (data_in[98] << 8) + data_in[99]));
    printf("Minimum Erase Count: %ld\n",
           (long)((data_in[106] << 24) + (data_in[107] << 16) +
                  (data_in[100] << 8) + data_in[101]));
    printf("Maximum Erase Count: %ld\n",
           (long)((data_in[108] << 24) + (data_in[109] << 16) +
                  (data_in[102] << 8) + data_in[103]));

    printf("Power Up Count: %ld\n",
           (long)((data_in[112] << 24) + (data_in[113] << 16) +
                  (data_in[114] << 8) + data_in[115]));
    printf("Abnormal Power Off Count: %d\n",
           (int)((data_in[128] << 8) + data_in[129]));
    printf("Total Refresh Count: %d\n",
           (int)((data_in[160] << 8) + data_in[161]));
    char tmpstr[100];
    strncpy(tmpstr, (char *)&data_in[176], 8);
    tmpstr[8] = 0;
    printf("Product Marker: %s\n", tmpstr);
    // sum up to get later bad block count
    sum = 0;
    for (int i = 184; i < 216; i++) {
      sum += data_in[i];
    }
    printf("Later Bad Block Count: %ld\n", sum);
    return true;
  } else {
    return false;
  }
}