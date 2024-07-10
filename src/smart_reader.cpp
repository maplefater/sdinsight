#include "smart_reader.hpp"
#include "mmc_utils.hpp"
#include "smart_reader_adata_by_transcend.hpp"
#include "smart_reader_apacer.hpp"
#include "smart_reader_longsys_m9h.hpp"
#include "smart_reader_longsys_mps.hpp"
#include "smart_reader_micron.hpp"
#include "smart_reader_sandisk.hpp"
#include "smart_reader_adata.hpp"
#include "smart_reader_swissbit.hpp"
#include "smart_reader_transcend.hpp"
template <typename T>
concept isSmartReader = requires(T t, MMCutils mmc_utils) {
  { T::readSmart(mmc_utils) } -> std::convertible_to<bool>;
};

template <typename T, typename... Args>
  requires isSmartReader<T>
struct readSmartImpl {
  bool operator()(MMCutils &mmc_utils) {
    return readSmartImpl<T>{}(mmc_utils) ||
           readSmartImpl<Args...>{}(mmc_utils);
  }
};
template <typename T>
  requires isSmartReader<T>
struct readSmartImpl<T> {
  bool operator()(MMCutils &mmc_utils) { return T::readSmart(mmc_utils); }
};
bool readSmart(MMCutils &mmc_utils) {
  return readSmartImpl<SmartReaderSandisk,
                       SmartReaderLongsysMPS,
                       SmartReaderMicron,
                       SmartReaderAdata,
                       SmartReaderTranscend,
                       SmartReaderSwissbit,
                       SmartReaderAdataByTranscend,
                       SmartReaderLongsysM9H,
                       SmartReaderApacer /* should be last one, to reduce execution time */>{}(mmc_utils);
}