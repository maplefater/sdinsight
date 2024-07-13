#ifndef SRC_SMART_READER_ATP_HPP
#define SRC_SMART_READER_ATP_HPP
#include "mmc_utils.hpp"

struct SmartReaderATP {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_ATP_HPP