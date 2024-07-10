#ifndef SRC_SMART_READER_LONGSYS_M9H_HPP
#define SRC_SMART_READER_LONGSYS_M9H_HPP
#include "mmc_utils.hpp"

struct SmartReaderLongsysM9H {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_LONGSYS_M9H_HPP