#include "smi_flash_id.hpp"
#include "mmc.h"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/mmc/ioctl.h>
#include <sstream>
#include <strings.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 256
#include "magic_enum/magic_enum_all.hpp"
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
    std::cout << "MMC_READ_SINGLE_BLOCK read failed: " << strerror(errno)
              << std::endl;
    return false;
  }
  return true;
}

bool writeBlock(int fd, void *buffer, int offset) {
  struct mmc_ioc_cmd idata;
  memset(&idata, 0, sizeof(idata));
  idata.write_flag = 1;
  idata.opcode = MMC_WRITE_BLOCK;
  idata.arg = offset / 512; // sector offset
  idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
  idata.blksz = 512;
  idata.blocks = 1;
  mmc_ioc_cmd_set_data(idata, buffer);
  auto tmp_ret = ioctl(fd, MMC_IOC_CMD, &idata);
  if (tmp_ret < 0) {
    std::cout << "MMC_WRITE_BLOCK read failed: " << strerror(errno)
              << std::endl;
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

enum class flashManufacturer {
  Spansion = 1,
  Fujitsu = 4,
  Renesas = 7,
  STMicro = 0x20,
  Micron = 0x2C,
  Sandisk = 0x45,
  SMIC = 0x4A,
  Qimonda = 0x51,
  Intel = 0x89,
  ESMT_PowerChip = 0x92,
  Toshiba = 0x98,
  YMTC = 0x9B,
  Hynix = 0xAD,
  Spectek = 0xB5,
  Macronix = 0xC2,
  ESMT_MIRA_PSC = 0xC8,
  Samsung = 0xEC,
  Winbond = 0xEF,
};

enum class toshibaAndSandiskType {
  _A19nm = 0,
  _15nm = 1,
  _70nm = 2,
  _56nm = 3,
  _43nm = 4,
  _32nm = 5,
  _24nm = 6,
  _19nm = 7,
  _48L_BiCS2 = 0x21,
  _64L_BiCS3 = 0x22,
  _96L_BiCS4 = 0x23,
  _112L_BiCS5 = 0x24,
  _162L_BiCS6 = 0x25,
};

enum class samsungType {
  _51nm = 0x40,
  _42nm = 0x41,
  _32nm = 0x42,
  _27nm = 0x43,
  _21nm = 0x44,
  _19nm = 0x45,
  _16nm = 0x46,
  _3dv1_24L = 0x47,
  _3dv2_32L = 0x48,
  _3dv3_48L = 0x49,
  _14nm = 0x4A,
  _3dv4_64L = 0x4B,
  _3dv5_92L = 0x4C,
  _3dv6_136L = 0x4D,
  _3dv7 = 0x4E,

};

char g_micron_types[] = {
    0x01, 0x04, 0x48, 0x00, 0x26, 0x89, 0x00, 0x00, 0x20, 0x33, 0x34, 0x6E,
    0x6D, 0x28, 0x4D, 0x36, 0x32, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x01, 0xA6, 0x89, 0x00, 0x00, 0x20, 0x33, 0x34, 0x6E,
    0x6D, 0x28, 0x4C, 0x36, 0x33, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x04, 0x46, 0x89, 0x00, 0x00, 0x20, 0x33, 0x34, 0x6E,
    0x6D, 0x28, 0x4C, 0x36, 0x33, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x88, 0x05, 0xC6, 0x89, 0x00, 0x00, 0x20, 0x33, 0x34, 0x6E,
    0x6D, 0x28, 0x4C, 0x36, 0x33, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x04, 0x46, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4D, 0x37, 0x33, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x00, 0x27, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4D, 0x37, 0x33, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x88, 0x01, 0xA7, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4D, 0x37, 0x33, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x20, 0x27, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4C, 0x37, 0x33, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x04, 0x4A, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4C, 0x37, 0x33, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x88, 0x05, 0xCA, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4C, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x05, 0x88, 0x04, 0x4B, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4C, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x05, 0xA8, 0x05, 0xCB, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x4C, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x05, 0x88, 0x24, 0x4B, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x05, 0x88, 0x24, 0x4B, 0xA9, 0x84, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x64, 0x44, 0x4B, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0xC5, 0x4B, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x34, 0x43, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x64, 0x64, 0x3C, 0xA1, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x34, 0x43, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x64, 0x64, 0x3C, 0xA5, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x34, 0x43, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0xE5, 0x3C, 0xA5, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x35, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x64, 0x3C, 0xA5, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x35, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0xE5, 0x3C, 0xA5, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x35, 0x43, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x64, 0x3C, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x4C, 0x38, 0x35, 0x43, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0xE5, 0x3C, 0xA9, 0x00, 0x00, 0x20, 0x31, 0x36, 0x6E,
    0x6D, 0x28, 0x4C, 0x39, 0x35, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x64, 0x54, 0xA9, 0x00, 0x00, 0x20, 0x31, 0x36, 0x6E,
    0x6D, 0x28, 0x4C, 0x39, 0x35, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0xE5, 0x54, 0xA9, 0x00, 0x00, 0x20, 0x33, 0x34, 0x6E,
    0x6D, 0x28, 0x42, 0x36, 0x33, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x68, 0x08, 0x56, 0x8A, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x42, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x88, 0x08, 0x5F, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x42, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x88, 0x08, 0x5F, 0x89, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x42, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x88, 0x28, 0x5F, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x42, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA8, 0x09, 0xDF, 0x89, 0x00, 0x00, 0x20, 0x32, 0x35, 0x6E,
    0x6D, 0x28, 0x42, 0x37, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA8, 0x09, 0xDF, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x42, 0x38, 0x35, 0x54, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x78, 0x63, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x42, 0x38, 0x35, 0x54, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x78, 0x7B, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x42, 0x38, 0x35, 0x54, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0xF9, 0x63, 0xA9, 0x00, 0x00, 0x20, 0x32, 0x30, 0x6E,
    0x6D, 0x28, 0x42, 0x38, 0x35, 0x54, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0xF9, 0x7B, 0xA9, 0x00, 0x00, 0x20, 0x31, 0x36, 0x6E,
    0x6D, 0x28, 0x42, 0x39, 0x35, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x48, 0x63, 0xA9, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x4C, 0x30, 0x34, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x64, 0x44, 0x32, 0xA5, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x4C, 0x30, 0x35, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x44, 0x34, 0xAA, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x42, 0x30, 0x35, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0x84, 0x58, 0x32, 0xA1, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x4C, 0x30, 0x35, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0x64, 0x34, 0xAA, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x4C, 0x30, 0x36, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0xE4, 0x34, 0x8A, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x4C, 0x30, 0x36, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0x64, 0x32, 0xAA, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x4C, 0x30, 0x36, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC4, 0xE5, 0x32, 0xAA, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x42, 0x30, 0x4B, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xB4, 0x78, 0x32, 0xAA, 0x00, 0x00, 0x20, 0x33, 0x32, 0x4C,
    0x28, 0x42, 0x30, 0x4B, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xCC, 0xF9, 0x32, 0xAA, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x42, 0x31, 0x36, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0x08, 0x32, 0xA1, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x42, 0x31, 0x36, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA4, 0x88, 0x32, 0xA1, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x42, 0x31, 0x36, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC4, 0x89, 0x32, 0xA1, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x42, 0x31, 0x37, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC4, 0x08, 0x32, 0xA6, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x42, 0x31, 0x37, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD4, 0x89, 0x32, 0xA6, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x42, 0x31, 0x37, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE4, 0x8A, 0x32, 0xA6, 0x00, 0x00, 0x20, 0x36, 0x34, 0x4C,
    0x28, 0x4E, 0x31, 0x38, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD4, 0x0C, 0x32, 0xAA, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x42, 0x32, 0x37, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC4, 0x18, 0x32, 0xA2, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x42, 0x32, 0x37, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD4, 0x99, 0x32, 0xA2, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x42, 0x32, 0x37, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE4, 0x9A, 0x32, 0xA2, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x42, 0x32, 0x37, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC3, 0x08, 0x32, 0xE6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x42, 0x32, 0x37, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD3, 0x89, 0x32, 0xE6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x42, 0x32, 0x37, 0x42, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE3, 0x8A, 0x32, 0xE6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x4E, 0x32, 0x38, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD3, 0x1C, 0x32, 0xC6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x4E, 0x32, 0x38, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD3, 0x9C, 0x32, 0xC6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x4E, 0x32, 0x38, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE3, 0x9D, 0x32, 0xC6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x4E, 0x32, 0x38, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xF3, 0x9E, 0x32, 0xC6, 0x00, 0x00, 0x20, 0x39, 0x36, 0x4C,
    0x28, 0x4D, 0x32, 0x36, 0x41, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA3, 0x60, 0x32, 0xC6, 0x00, 0x00, 0x20, 0x31, 0x32, 0x38,
    0x4C, 0x28, 0x42, 0x33, 0x36, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xA3, 0x78, 0x32, 0xE5, 0x00, 0x00, 0x20, 0x31, 0x32, 0x38,
    0x4C, 0x28, 0x42, 0x33, 0x37, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC3, 0x78, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x31, 0x37, 0x36,
    0x4C, 0x28, 0x42, 0x34, 0x37, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xC3, 0x08, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x31, 0x37, 0x36,
    0x4C, 0x28, 0x42, 0x34, 0x37, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD3, 0x89, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x31, 0x37, 0x36,
    0x4C, 0x28, 0x42, 0x34, 0x37, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE3, 0x8A, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x31, 0x37, 0x36,
    0x4C, 0x28, 0x4E, 0x34, 0x38, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD3, 0x0C, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x31, 0x37, 0x36,
    0x4C, 0x28, 0x4E, 0x34, 0x38, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE3, 0x8D, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x31, 0x37, 0x36,
    0x4C, 0x28, 0x4E, 0x34, 0x38, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xF3, 0x8E, 0x32, 0xEA, 0x00, 0x00, 0x20, 0x32, 0x33, 0x32,
    0x4C, 0x28, 0x42, 0x35, 0x38, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xD3, 0x08, 0x32, 0xE8, 0x00, 0x00, 0x20, 0x32, 0x33, 0x32,
    0x4C, 0x28, 0x42, 0x35, 0x38, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xE3, 0x89, 0x32, 0xE8, 0x00, 0x00, 0x20, 0x32, 0x33, 0x32,
    0x4C, 0x28, 0x42, 0x35, 0x38, 0x52, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x04, 0xF3, 0x8A, 0x32, 0xE8, 0x00, 0x00, 0x20, 0x31, 0x34, 0x34,
    0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x89, 0xD3, 0xAC, 0x32, 0xC6, 0x00, 0x20, 0x31, 0x34, 0x34,
    0x4C, 0x00, 0x00, 0x00};

// first byte of flash id
std::string parseFlashId(char *buffer) {
  std::stringstream ss;
  auto manufacturer = (flashManufacturer)(buffer[0]);
  // manufacturer name
  if (magic_enum::enum_contains(manufacturer)) {
    ss << magic_enum::enum_name(manufacturer);
  } else {
    ss << "Unknown";
    return ss.str();
  }

  // process node type
  if (manufacturer == flashManufacturer::Sandisk ||
      manufacturer == flashManufacturer::Toshiba) {
    auto type = (toshibaAndSandiskType)(buffer[5] & 0x27);
    if (magic_enum::enum_contains(type)) {
      auto type_str = magic_enum::enum_name(type);
      type_str.remove_prefix(1);
      ss << " " << type_str;
    }
  }
  if (manufacturer == flashManufacturer::Samsung) {
    auto type = (samsungType)(buffer[5] & 0x7F);
    if (magic_enum::enum_contains(type)) {
      auto type_str = magic_enum::enum_name(type);
      type_str.remove_prefix(1);
      ss << " " << type_str;
    }
  }

  if (manufacturer == flashManufacturer::Hynix) {
    switch (buffer[5]) {
    case 0x25:
    case 0x40:
      ss << " 16nm";
      break;
    case 0x41:
      ss << " 41nm";
      break;
    case 0x42:
      ss << " 32nm";
      break;
    case 0x43:
      ss << " 26nm";
      break;
    case 0x44:
      ss << " 20nm";
      break;
    case 0x45:
    case 0x48:
    case 0x49:
    case 0x4A:
      ss << " 16nm";
      break;
    case 0x50:
      ss << " 14nm";
      break;
    case 0x60:
      ss << " 3DV1";
      break;
    case 0x65:
      ss << " 16nm";
      break;
    case 0x70:
      ss << " 3dv2_36L";
      break;
    case 0x80:
      ss << " 3dv3_48L";
      break;
    case 0x90:
      ss << " 3dv4_72L";
      break;
    case 0xA0:
    case 0xA2:
      ss << " 3dv5_96L";
      break;
    case 0xB0:
    case 0xB2:
      ss << " 3dv6_128L";
      break;
    case 0xC0:
      ss << " 3dv7_176L";
      break;
    case 0xC3:
      ss << " 26nm";
      break;
    case 0xC4:
      ss << " 20nm";
      break;
    case 0xD0:
      ss << " 3dv8_238L";
      break;
    case 0xE0:
      ss << " 14nm";
      break;
    case 0xE5:
      ss << " 16nm";
      break;
    default:
      break;
    }
  }

  if (manufacturer == flashManufacturer::YMTC) {
    // todo
  }

  if (manufacturer == flashManufacturer::Micron ||
      manufacturer == flashManufacturer::Intel ||
      manufacturer == flashManufacturer::Spectek) {
    struct micronType {
      uint8_t buffer_offset;
      uint8_t pattern_length;
      uint8_t pattern[6];
      char name[16];
    };
    int micron_types_size = sizeof(g_micron_types) / 24;
    for (int i = 0; i < micron_types_size; i++) {
      micronType *single_pattern = (micronType *)(g_micron_types + i * 24);
      if (memcmp(buffer + single_pattern->buffer_offset, single_pattern->pattern,
                 single_pattern->pattern_length) == 0) {
        ss << single_pattern->name;
        break;
      }
    }
  }

  // cell type
  if (manufacturer == flashManufacturer::YMTC) {
    if (buffer[1] == 0xD5 && buffer[2] == 88 && buffer[3] == 0x8D &&
        buffer[4] == 32) {
      ss << " QLC";
    }
  } else {
    switch ((buffer[2] >> 2) & 3) {
    case 0:
      ss << " SLC";
      break;
    case 1:
      ss << " MLC";
      break;
    case 2:
      ss << " TLC";
      break;
    case 3:
      ss << " QLC";
      break;
    default:
      break;
    }
  }

  return ss.str();
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
  bool sm2706_or_sth = false;
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
    sm2706_or_sth = true;
  }

  if (controller_name == "unknown") {
    std::cout << "Unknown controller name." << std::endl;
    return;
  }
  std::cout << "SD Controller: " << controller_name << std::endl;
  std::cout << "SD Firmware: " << firmware_name << std::endl;

  // read flash id
  int flash_id_offset = 4;
  memset(buffer, 0, 0x200);
  if (sm2706_or_sth) {
    flash_id_offset = 0;
    buffer[0] = 6;
  } else {
    buffer[0] = 0xF0;
    buffer[1] = 6;
  }
  if (!writeBlock(fd, buffer, 0xAB5400)) {
    printf(
        "Possible disk has partition within first 12MB.\nFID inaccessible\n");
  }
  CHECK_RET(readBlock(fd, buffer, 0xAB5400));
  for (int i = flash_id_offset; i < flash_id_offset + 16; i++) {
    auto ret = parseFlashId(buffer + 8 * i);
    if (buffer[8 * i] == 0) {
      continue;
    }
    printf("Bank%02u: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x - %s\n",
           i - flash_id_offset, buffer[8 * i + 0], buffer[8 * i + 1],
           buffer[8 * i + 2], buffer[8 * i + 3], buffer[8 * i + 4],
           buffer[8 * i + 5], buffer[8 * i + 6], buffer[8 * i + 7],
           ret.c_str());
  }
}