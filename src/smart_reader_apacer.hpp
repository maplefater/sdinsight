#ifndef SRC_SMART_READER_APACER_HPP
#define SRC_SMART_READER_APACER_HPP
#include "mmc_utils.hpp"

struct SmartReaderApacer {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_APACER_HPP