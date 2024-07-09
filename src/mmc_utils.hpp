#ifndef SRC_MMC_UTILS_HPP
#define SRC_MMC_UTILS_HPP

#include <fcntl.h>
#include <linux/mmc/ioctl.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>

#include <vector>

class MMCutils {
private:
  static const int SD_BLOCK_SIZE = 512;

public:
  MMCutils(std::string dev_path);
  std::vector<uint8_t> getCid();
  std::vector<uint8_t> CMD56_read(int arg);

  int CMD56_write(int arg);
  int bytes_to_int(unsigned char byte1, unsigned char byte2,
                   unsigned char byte3, unsigned char byte4);

  int nword_to_int(const std::vector<uint8_t> &data, int offset, int size);

private:
  int m_fd = -1;
};

#endif // SRC_MMC_UTILS_HPP