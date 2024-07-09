#include "smart_reader.hpp"
#include "mmc_utils.hpp"
#include "smart_reader_longsys_0x1.hpp"
#include "smart_reader_sandisk.hpp"
template <typename T>
concept isSmartReader = requires(T t, MMCutils mmc_utils) {
  { T::readSmart(mmc_utils) } -> std::convertible_to<bool>;
};

template <typename T>
  requires isSmartReader<T>
bool readSmartImpl(MMCutils &mmc_utils) {
  return T::readSmart(mmc_utils);
}

template <typename T, typename... Args>
  requires isSmartReader<T>
bool readSmartInternal(MMCutils &mmc_utils) {
  // stop when success
  return readSmartImpl<T>(mmc_utils) || readSmartImpl<Args...>(mmc_utils);
}

bool readSmart(MMCutils &mmc_utils) {
  return readSmartInternal<SmartReaderSandisk, SmartReaderLongsys0x1>(
      mmc_utils);
}