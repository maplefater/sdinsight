#ifndef SRC_SMART_READER_LONGSYS_MPS_HPP
#define SRC_SMART_READER_LONGSYS_MPS_HPP
#include "mmc_utils.hpp"

struct SmartReaderLongsysMPS {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_LONGSYS_MPS_HPP