#ifndef SRC_SMART_READER_ADATA_HPP
#define SRC_SMART_READER_ADATA_HPP
#include "mmc_utils.hpp"

struct SmartReaderAdata {
  static bool readSmart(MMCutils &mmc_utils);
};

#endif // SRC_SMART_READER_ADATA_HPP