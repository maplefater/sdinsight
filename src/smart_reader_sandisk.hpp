#ifndef SRC_SMART_READER_SANDISK_HPP
#define SRC_SMART_READER_SANDISK_HPP
#include "mmc_utils.hpp"

struct SmartReaderSandisk {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_SANDISK_HPP