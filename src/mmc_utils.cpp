#include "mmc_utils.hpp"
#include "mmc.h"
#include <errno.h>
#include <iostream>
std::vector<uint8_t> MMCutils::CMD56_read(int arg) {
  std::vector<uint8_t> ret;
  ret.resize(SD_BLOCK_SIZE);
  struct mmc_ioc_cmd idata;
  uint8_t *lba_block_data = ret.data();
  memset(&idata, 0, sizeof(idata));
  memset(lba_block_data, 0, sizeof(uint8_t) * SD_BLOCK_SIZE);

  idata.write_flag = 0;
  idata.opcode = MMC_GEN_CMD;
  idata.arg = arg;
  idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
  idata.blksz = SD_BLOCK_SIZE;
  idata.blocks = 1;
  mmc_ioc_cmd_set_data(idata, lba_block_data);
  auto tmp_ret = ioctl(m_fd, MMC_IOC_CMD, &idata);
  if (tmp_ret < 0) {
    std::cout << "cmd56 read failed: " << strerror(errno);
    exit(1);
  }
  return ret;
}
int MMCutils::CMD56_write(int arg) {
  int ret = 0;
  struct mmc_ioc_cmd idata;
  char data_out[SD_BLOCK_SIZE];

  memset(&idata, 0, sizeof(idata));
#if defined(__linux__)
  memset(&data_out[0], 0, sizeof(__u8) * SD_BLOCK_SIZE);
#elif defined(__FreeBSD__)
  memset(&data_out[0], 0, sizeof(SD_BLOCK_SIZE));
#endif

  idata.write_flag = 1;
  idata.opcode = MMC_GEN_CMD;
  idata.arg = arg;

  // data_in[0]=0x10; // also cmd56_arg macht keinen Unterschied ...

  // idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC | MMC_CMD_ADTC;
  // //this works (and that matches the spec. Table 4.29 of
  // Part1_Physical_Layer_Simplified_Specification_Ver6.00.pdf)
  idata.flags =
      MMC_RSP_R1 |
      MMC_CMD_ADTC; // this works (and that matches the spec. Table 4.29 of
                    // Part1_Physical_Layer_Simplified_Specification_Ver6.00.pdf)
  // this looks better and works also
  // idata.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;

  idata.blksz = SD_BLOCK_SIZE;
  idata.blocks = 1; // set to 0 NO  -> like now send an empty block with 0 and
                    // correct block size
  mmc_ioc_cmd_set_data(idata, data_out);

  // idata.data_timeout_ns=4000000000; // 4s this is nearly max, but it has no
  // consequence idata.cmd_timeout_ms=20000;  // 20s, but it has no
  // consequence idata.cmd_timeout_ms = 10000; //set 10 sec

  ret = ioctl(m_fd, MMC_IOC_CMD, &idata);
  //  if (ret)
  //    printf("ioctl errror\n");

  // see idata.response[0] -[3] but it is 0
  printf("\"idata.response[]\":\"0x%02x 0x%02x 0x%02x 0x%02x\",\n",
         idata.response[0], idata.response[1], idata.response[2],
         idata.response[3]);

  return ret;
}
MMCutils::MMCutils(std::string dev_path) {
  m_fd = open(dev_path.c_str(), O_RDWR);
  if (m_fd < 0) {
    std::cout << "open device failed: " << strerror(errno);
    exit(1);
  }
}
int MMCutils::bytes_to_int(unsigned char byte1, unsigned char byte2,
                           unsigned char byte3, unsigned char byte4) {
  return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
int MMCutils::nword_to_int(const std::vector<uint8_t> &data, int offset,
                           int size) {
  if (size == 4) {
    return ((data[offset + 3] << 24) | (data[offset + 2] << 16) |
            (data[offset + 1] << 8) | data[offset]);
  } else if (size == 8) {
    return (((long long)data[offset + 3] << 56) |
            ((long long)data[offset + 3] << 48) |
            ((long long)data[offset + 3] << 40) |
            ((long long)data[offset + 3] << 32) |
            ((long long)data[offset + 3] << 24) |
            ((long long)data[offset + 2] << 16) |
            ((long long)data[offset + 1] << 8) | (long long)data[offset]);
  } else {
    return -1;
  }
}
