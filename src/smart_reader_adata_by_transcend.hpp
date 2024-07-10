#ifndef SRC_SMART_READER_ADATA_BY_TRANSCEND_HPP
#define SRC_SMART_READER_ADATA_BY_TRANSCEND_HPP
#include "mmc_utils.hpp"

struct SmartReaderAdataByTranscend {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_ADATA_BY_TRANSCEND_HPP