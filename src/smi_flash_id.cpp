#include "smi_flash_id.hpp"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include "mmc.h"
#include <linux/mmc/ioctl.h>
#include <sys/ioctl.h>
static int g_sequence = 0;

#define CHECK_RET(arg)                                                         \
  if (!(arg)) {                                                                \
    std::cout << "Error[" << g_sequence << "] " << #arg << std::endl;          \
    return;                                                                    \
  }                                                                            \
  g_sequence++



bool readBlock(int fd, void *buffer, int offset) {
  struct mmc_ioc_cmd idata;
  memset(&idata, 0, sizeof(idata));
  memset(buffer, 0, 0x200);
  idata.write_flag = 0;
  idata.opcode = MMC_READ_SINGLE_BLOCK;
  idata.arg = offset / 512; // sector offset
  idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
  idata.blksz = 512;
  idata.blocks = 1;
  mmc_ioc_cmd_set_data(idata, buffer);
  auto tmp_ret = ioctl(fd, MMC_IOC_CMD, &idata);
  if (tmp_ret < 0) {
    std::cout << "MMC_READ_SINGLE_BLOCK read failed: " << strerror(errno) << std::endl;
    return false;
  }
  return true;
}

[[maybe_unused]]
void printBuffer(char *buffer) {
  for (int i = 0; i < 512; i++) {
    if (i != 0 && i % 16 == 0) {
      std::cout << std::endl;
    }
    printf("%02X ", buffer[i]);
  }
  std::cout << std::endl << std::endl;
}



void printSmiFlashId() {
  int fd = open("/dev/mmcblk0", O_RDWR | O_SYNC);
  if (fd < 0) {
    std::cout << "open /dev/mmcblk0 failed." << std::endl;
    return;
  }
  char *buffer = (char *)malloc(0x200);
  char tmpstr[100];
  std::string controller_name = "unknown";
  std::string firmware_name = "unknown";
  // magic from smi_sd_flash_id.exe
  CHECK_RET(readBlock(fd, buffer, 0xAB5400));
  CHECK_RET(readBlock(fd, buffer, 0x15400));
  CHECK_RET(readBlock(fd, buffer, 0x1540000));
  CHECK_RET(readBlock(fd, buffer, 0xAA00));
  CHECK_RET(readBlock(fd, buffer, 0xAA0000));
  CHECK_RET(readBlock(fd, buffer, 0xAB5400));
  if (buffer[8] == 83 && buffer[9] == 77) {
    strncpy(tmpstr, (char *)&buffer[8], 8);
    tmpstr[8] = 0;
    controller_name = tmpstr;
    strncpy(tmpstr, (char *)&buffer[0], 8);
    tmpstr[8] = 0;
    firmware_name = tmpstr;
  }
  if (buffer[8] == 95 && buffer[9] == 83 && buffer[10] == 77) {
    strncpy(tmpstr, (char *)&buffer[9], 7);
    tmpstr[8] = 0;
    controller_name = tmpstr;
    strncpy(tmpstr, (char *)&buffer[0], 8);
    tmpstr[8] = 0;
    firmware_name = tmpstr;
  }
  if (buffer[0] == 83 && buffer[1] == 77) {
    strncpy(tmpstr, (char *)&buffer[0], 6);
    tmpstr[6] = 0;
    controller_name = tmpstr;
  }
  if (controller_name == "unknown") {
    std::cout << "Unknown controller name." << std::endl;
    return;
  }
  std::cout << "SD Controller: " << controller_name << std::endl;
  std::cout << "SD Firmware: " << firmware_name << std::endl;

}