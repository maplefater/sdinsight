#ifndef SRC_SMART_READER_MICRON_HPP
#define SRC_SMART_READER_MICRON_HPP
#include "mmc_utils.hpp"

struct SmartReaderMicron {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_MICRON_HPP