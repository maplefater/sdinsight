#ifndef SRC_SMART_READER_SWISSBIT_HPP
#define SRC_SMART_READER_SWISSBIT_HPP
#include "mmc_utils.hpp"

struct SmartReaderSwissbit {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_SWISSBIT_HPP