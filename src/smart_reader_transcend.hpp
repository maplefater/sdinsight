#ifndef SRC_SMART_READER_TRANSCEND_HPP
#define SRC_SMART_READER_TRANSCEND_HPP
#include "mmc_utils.hpp"

struct SmartReaderTranscend {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_TRANSCEND_HPP