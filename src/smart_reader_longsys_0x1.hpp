#ifndef SRC_SMART_READER_LONGSYS_0X1_HPP
#define SRC_SMART_READER_LONGSYS_0X1_HPP
#include "mmc_utils.hpp"

struct SmartReaderLongsys0x1 {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_LONGSYS_0X1_HPP