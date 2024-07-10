#include "mmc_utils.hpp"
#include "mmc.h"
#include <algorithm>
#include <climits>
#include <cstdarg>
#include <errno.h>
#include <filesystem>
#include <iostream>
#include <stacktrace>
std::vector<uint8_t> MMCutils::CMD56_read(int arg) {
  std::vector<uint8_t> ret;
  ret.resize(SD_BLOCK_SIZE);
  struct mmc_ioc_cmd idata;
  uint8_t *lba_block_data = ret.data();
  memset(&idata, 0, sizeof(idata));
  memset(lba_block_data, 0, sizeof(uint8_t) * SD_BLOCK_SIZE);

  idata.write_flag = 0;
  idata.opcode = MMC_GEN_CMD;
  idata.arg = arg;
  idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
  idata.blksz = SD_BLOCK_SIZE;
  idata.blocks = 1;
  mmc_ioc_cmd_set_data(idata, lba_block_data);
  auto tmp_ret = ioctl(m_fd, MMC_IOC_CMD, &idata);
  if (tmp_ret < 0) {
    std::cout << "cmd56 read failed: " << strerror(errno) << std::endl;
    std::cout << std::stacktrace::current() << std::endl;
    exit(1);
  }
  return ret;
}
int MMCutils::CMD56_write(int arg) {
  int ret = 0;
  struct mmc_ioc_cmd idata;
  char data_out[SD_BLOCK_SIZE];

  memset(&idata, 0, sizeof(idata));
#if defined(__linux__)
  memset(&data_out[0], 0, sizeof(__u8) * SD_BLOCK_SIZE);
#elif defined(__FreeBSD__)
  memset(&data_out[0], 0, sizeof(SD_BLOCK_SIZE));
#endif

  idata.write_flag = 1;
  idata.opcode = MMC_GEN_CMD;
  idata.arg = arg;

  // data_in[0]=0x10; // also cmd56_arg macht keinen Unterschied ...

  // idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC | MMC_CMD_ADTC;
  // //this works (and that matches the spec. Table 4.29 of
  // Part1_Physical_Layer_Simplified_Specification_Ver6.00.pdf)
  idata.flags =
      MMC_RSP_R1 |
      MMC_CMD_ADTC; // this works (and that matches the spec. Table 4.29 of
                    // Part1_Physical_Layer_Simplified_Specification_Ver6.00.pdf)
  // this looks better and works also
  // idata.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;

  idata.blksz = SD_BLOCK_SIZE;
  idata.blocks = 1; // set to 0 NO  -> like now send an empty block with 0 and
                    // correct block size
  mmc_ioc_cmd_set_data(idata, data_out);

  // idata.data_timeout_ns=4000000000; // 4s this is nearly max, but it has no
  // consequence idata.cmd_timeout_ms=20000;  // 20s, but it has no
  // consequence idata.cmd_timeout_ms = 10000; //set 10 sec

  ret = ioctl(m_fd, MMC_IOC_CMD, &idata);

  return ret;
}
MMCutils::MMCutils() {
  m_fd = open("/dev/mmcblk0", O_RDWR);
  if (m_fd < 0) {
    std::cout << "open device failed: " << strerror(errno);
    exit(1);
  }
  // for some reason, cid and csd can't be retrieved from /dev/mmcblk0
  // use bus folder instead (cid and csd provided by mmc driver)
  const std::filesystem::path mmc_devices{"/sys/bus/mmc/devices/"};
  std::ranges::for_each(
      std::filesystem::directory_iterator{mmc_devices},
      [this](const auto &dir_entry) {
        if (dir_entry.path().filename().string().find("mmc0") !=
            std::string::npos) {
          m_bus_folder = dir_entry;
          std::cout << "/dev/mmcblk0 device bus: " << m_bus_folder << std::endl;
        }
      });
}
int MMCutils::bytes_to_int(unsigned char byte1, unsigned char byte2,
                           unsigned char byte3, unsigned char byte4) {
  return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
int MMCutils::nword_to_int(const std::vector<uint8_t> &data, int offset,
                           int size) {
  if (size == 4) {
    return ((data[offset + 3] << 24) | (data[offset + 2] << 16) |
            (data[offset + 1] << 8) | data[offset]);
  } else if (size == 8) {
    return (((long long)data[offset + 7] << 56) |
            ((long long)data[offset + 6] << 48) |
            ((long long)data[offset + 5] << 40) |
            ((long long)data[offset + 4] << 32) |
            ((long long)data[offset + 3] << 24) |
            ((long long)data[offset + 2] << 16) |
            ((long long)data[offset + 1] << 8) | (long long)data[offset]);
  } else {
    return -1;
  }
}
int64_t MMCutils::nword_to_int64(const std::vector<uint8_t> &data, int offset) {
  return (
      ((int64_t)data[offset + 7] << 56) | ((int64_t)data[offset + 6] << 48) |
      ((int64_t)data[offset + 5] << 40) | ((int64_t)data[offset + 4] << 32) |
      ((int64_t)data[offset + 3] << 24) | ((int64_t)data[offset + 2] << 16) |
      ((int64_t)data[offset + 1] << 8) | (int64_t)data[offset]);
}

static std::string get_manufacturer(int manid, char *oemid) {
  switch (manid) {
  case 0x00:
    return "Fake";
  case 0x01:
    return "Panasonic";

  case 0x02:
    return "Toshiba/Kingston/Viking";

  case 0x03:
    if (!memcmp(oemid, "DW", 2))
      return "Western Digital"; // WD.
    else
      return "SanDisk";

  case 0x06:
    return "Ritek";

  case 0x08:
    return "Silicon Power";
  case 0x09:
    return "ATP";

  case 0x13:
    return "Kingmax";

  case 0x18:
    return "Infineon";
  case 0x19:
    return "Dynacard";

  case 0x1A:
    return "Power Quotient";

  case 0x1B:
    return "Transcend/Samsung";

  case 0x1C:
    return "Transcend";
  case 0x1D:
    return "Corsair/AData";

  case 0x1E:
    return "Transcend";
  case 0x1F:
    return "Kingston";
  case 0x22:
    return "Dahua";
  case 0x27:
    return "Delkin/Phison";
  case 0x28:
    return "Barun Electronics/Lexar";

  case 0x31:
    return "Silicon Power";
  case 0x33:
    return "STMicroelectronics";
  case 0x41:
    return "Kingston";
  case 0x51:
    return "STEC";
  case 0x5D:
    return "SwissBit";
  case 0x61:
    return "Netlist";
  case 0x63:
    return "Cactus";
  case 0x6F:
    return "STMicroelectronics";

  case 0x73:
    return "Bongiovi";
  case 0x74:
    return "Jiaelec/Transcend";
  case 0x76:
    return "Patriot";
  case 0x82:
    return "Jiang Tay/Gobe/Sony";
  case 0x83:
    return "Netcom";
  case 0x84:
    return "Strontium";
  case 0x9C:
    if (!memcmp(oemid, "OS", 2))
      return "Sony"; // SO.
    else if (!memcmp(oemid, "SO", 2))
      return "Hikvision";
    else
      return "Barun Electronics"; // BE.
  case 0x9F:
    return "Taishin";
  case 0xAD:
    return "Longsys";
  default:
    return "Unknown";
  }
  return "Unknown";
}

char *to_binstr(char *hexstr) {
  const char *bindigits[] = {
      "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
      "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111",
  };
  char *binstr, *tail;

  binstr = (char *)calloc(strlen(hexstr) * 4 + 1, sizeof(char));
  if (!binstr)
    return NULL;

  tail = binstr;

  while (hexstr && *hexstr != '\0') {
    if (!isxdigit(*hexstr)) {
      free(binstr);
      return NULL;
    }

    if (isdigit(*hexstr))
      strcat(tail, bindigits[*hexstr - '0']);
    else if (islower(*hexstr))
      strcat(tail, bindigits[*hexstr - 'a' + 10]);
    else
      strcat(tail, bindigits[*hexstr - 'A' + 10]);

    hexstr++;
    tail += 4;
  }

  return binstr;
}
void bin_to_unsigned(unsigned int *u, char *binstr, int width) {
  *u = 0;

  while (binstr && *binstr != '\0' && width > 0) {
    *u <<= 1;
    *u |= *binstr == '0' ? 0 : 1;

    binstr++;
    width--;
  }
}

void bin_to_ascii(char *a, char *binstr, int width) {
  *a = '\0';

  while (binstr && *binstr != '\0' && width > 0) {
    unsigned int u;
    char c[2] = {'\0', '\0'};
    char *s = &c[0];

    bin_to_unsigned(&u, binstr, 8);
    c[0] = u;

    strcat(a, s);
    binstr += 8;
    width -= 8;
  }
}

void parse_bin(char *hexstr, const char *fmt, ...) {
  va_list args;
  char *origstr;
  char *binstr;
  unsigned long width = 0;

  binstr = to_binstr(hexstr);
  origstr = binstr;

  va_start(args, fmt);

  while (binstr && fmt && *fmt != '\0') {
    if (isdigit(*fmt)) {
      char *rest;

      errno = 0;
      width = strtoul(fmt, &rest, 10);
      if (width == ULONG_MAX && errno != 0)
        fprintf(stderr, "strtoul()");
      fmt = rest;
    } else if (*fmt == 'u') {
      unsigned int *u = va_arg(args, unsigned int *);

      if (u)
        bin_to_unsigned(u, binstr, width);
      binstr += width;
      width = 0;
      fmt++;
    } else if (*fmt == 'r') {
      binstr += width;
      width = 0;
      fmt++;
    } else if (*fmt == 'a') {
      char *c = va_arg(args, char *);

      if (c)
        bin_to_ascii(c, binstr, width);
      binstr += width;
      width = 0;
      fmt++;
    } else {
      fmt++;
    }
  }

  va_end(args);
  free(origstr);
}

void print_sd_cid(char *cid) {
  unsigned int mid;
  char oid[3];
  char pnm[6];
  unsigned int hwrev;
  unsigned int fwrev;
  unsigned int psn;
  unsigned int mdt_month;
  unsigned int mdt_year;
  unsigned int crc;
  auto parse_format = "8u16a40a4u4u32u4r8u4u7u1r";
  parse_bin(cid, parse_format, &mid, &oid[0], &pnm[0],
            &hwrev, &fwrev, &psn, &mdt_year, &mdt_month, &crc);

  oid[2] = '\0';
  pnm[5] = '\0';

  auto manufacturer = get_manufacturer(mid, oid);
  printf("manufacturer: %s [0x%02X]\n", manufacturer.c_str(), mid);
  printf("OEM ID: %s\n", oid);
  printf("product: %s\n", pnm);
  printf("hwrev: %u\n", hwrev);
  printf("fwrev: %u\n", fwrev);
  printf("serial: 0x%08x\n", psn);
  printf("manufacturing date: %u/%u\n", 2000 + mdt_year, mdt_month);
}
char *read_file(const char *name) {
  char line[4096];
  char *preparsed, *start = line;
  int len;
  FILE *f;

  f = fopen(name, "r");
  if (!f) {
    fprintf(stderr, "Could not open MMC/SD file '%s'.\n", name);
    return NULL;
  }

  preparsed = fgets(line, sizeof(line), f);
  if (!preparsed) {
    if (ferror(f))
      fprintf(stderr, "Could not read MMC/SD file '%s'.\n", name);
    else
      fprintf(stderr, "Could not read data from MMC/SD file '%s'.\n", name);

    if (fclose(f))
      fprintf(stderr, "Could not close MMC/SD file '%s'.\n", name);
    return NULL;
  }

  if (fclose(f)) {
    fprintf(stderr, "Could not close MMC/SD file '%s'.\n", name);
    return NULL;
  }

  line[sizeof(line) - 1] = '\0';
  len = strlen(line);

  while (len > 0 && isspace(line[len - 1]))
    len--;

  while (len > 0 && isspace(*start)) {
    start++;
    len--;
  }

  start[len] = '\0';
  return strdup(start);
}

void MMCutils::printCid() {
  auto *cid = read_file((m_bus_folder / "cid").c_str());
  print_sd_cid(cid);
}
