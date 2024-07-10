#include "smart_reader_swissbit.hpp"

bool SmartReaderSwissbit::readSmart(MMCutils &mmc_utils) {
  auto data_in = mmc_utils.CMD56_read(0x53420001);
  if (data_in[0] == 0x53 && data_in[1] == 0x77) {
    printf("Device Model: Swissbit\n");
    printf(
        "Firmware Version: [%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c,%c]\n",
        data_in[32], data_in[33], data_in[34], data_in[35], data_in[36],
        data_in[37], data_in[38], data_in[39], data_in[40], data_in[41],
        data_in[42], data_in[43], data_in[44], data_in[45], data_in[46],
        data_in[47]); // show char
    printf("User area rated cycles: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[48], data_in[49], data_in[50], data_in[51]);
    printf("User area max cycle cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[52], data_in[53], data_in[54], data_in[55]);
    printf("User area total cycle cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[56], data_in[57], data_in[58], data_in[59]);
    printf("User area average cycle cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[60], data_in[61], data_in[62], data_in[63]);
    printf("System area max cycle cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[68], data_in[69], data_in[70], data_in[71]);
    printf("System area total cycle cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[72], data_in[73], data_in[74], data_in[75]);
    printf("System area average cycle cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[76], data_in[77], data_in[78], data_in[79]);
    printf("Remaining Lifetime Percent: %d%%\n", (int)(data_in[80]));
    switch (data_in[86]) {
    case 0x00:
      printf("Speed mode: Default speed\n");
      break;
    case 0x01:
      printf("Speed mode: High speed\n");
      break;
    case 0x10:
      printf("Speed mode: SDR12 speed\n");
      break;
    case 0x11:
      printf("Speed mode: SDR25 speed\n");
      break;
    case 0x12:
      printf("Speed mode: SDR50 speed\n");
      break;
    case 0x14:
      printf("Speed mode: DDR50 speed\n");
      break;
    case 0x18:
      printf("Speed mode: SDR104 speed\n");
      break;
    }
    switch (data_in[87]) {
    case 0x00:
      printf("Bus width: 1 bit\n");
      break;
    case 0x10:
      printf("Bus width: 4 bits\n");
      break;
    }
    printf("User area spare blocks cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[88], data_in[89], data_in[90], data_in[91]);
    printf("System area spare blocks cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[92], data_in[93], data_in[94], data_in[95]);
    printf("User area runtime bad blocks cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[96], data_in[97], data_in[98], data_in[99]);
    printf("System area runtime bad blocks cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[100], data_in[101], data_in[102], data_in[103]);
    printf("User area refresh cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[104], data_in[105], data_in[106], data_in[107]);
    printf("System area refresh cnt: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[108], data_in[109], data_in[110], data_in[111]);
    printf("Host Interface CRC count: "
           "[0x%02x0x%02x0x%02x0x%02x]\n",
           data_in[112], data_in[113], data_in[114], data_in[115]);
    printf("Power cycle count: [%d%d%d%d]\n",
           data_in[116], data_in[117], data_in[118], data_in[119]);
    return true;
  }
  return false;
}