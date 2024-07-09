#include "smart_reader_longsys_0x1.hpp"

bool SmartReaderLongsys0x1::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x1);
  if (data_in[0] == 0x70 && data_in[1] == 0x58) {
    printf("Longsys: true\n");
    printf("SMARTVersions: %d\n", mmc_utils.nword_to_int(data_in, 4, 4));
    printf("sizeOfDevSMART: %d\n", mmc_utils.nword_to_int(data_in, 12, 4));
    printf("originalBadBlock: %d\n", mmc_utils.nword_to_int(data_in, 16, 4));
    printf("increaseBadBlock: %d\n", mmc_utils.nword_to_int(data_in, 20, 4));
    printf("writeAllSectNum: %d Sector(512Byte)\n",
           mmc_utils.nword_to_int(data_in, 24, 8));
    printf("replaceBlockLeft: %d\n", mmc_utils.nword_to_int(data_in, 32, 4));
    printf("degreOfWear: %.02f Cycle\n",
           (float)mmc_utils.nword_to_int(data_in, 36, 4) / 1000);
    printf("sectorTotal: %d\n", mmc_utils.nword_to_int(data_in, 40, 4));
    printf("remainLifeTime: %d%%\n", mmc_utils.nword_to_int(data_in, 44, 4));
    printf("remainWrGBNum: %.02fTB\n",
           (float)mmc_utils.nword_to_int(data_in, 48, 4) / 1024);
    printf("lifeTimeTotal: %.02f Cycle\n",
           (float)mmc_utils.nword_to_int(data_in, 52, 4));
    printf("phyWrGBNum: %.02fTB\n",
           (float)mmc_utils.nword_to_int(data_in, 56, 4) / 1024);

    return true;
  } else {
    return false;
  }
}