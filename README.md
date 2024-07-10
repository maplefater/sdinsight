# sdinsight

Just another SD/TF card CID & SMART reader.

`/dev/mmcblk0` device is **required** .

SMART supported card type:

* AData
* AData by Transcend
* Apacer
* Sandisk
* Longsys M9H
* Longsys MPS
* Micron
* Swissbit
* Transcend

Tested on:

* raspberry pi 5 with Ubuntu 24.04


## Build

**C++23 required.**

Install build dependences:

```bash
sudo apt install build-essential cmake gcc gdb g++ ninja-build
```

Build:

```bash
mkdir build
cd build
cmake ..
cmake --build . -j 4
```

## Usage

```bash
# /dev/mmcblk0 is used
sudo ./sdinsight
```

## Examples

### Hikvision longsys MPS

```
/dev/mmcblk0 device bus: "/sys/bus/mmc/devices/mmc0:59b4"
====================CID info====================
manufacturer: Hikvision [0x9C]
OEM ID: SO
product: USD00
hwrev: 4
fwrev: 8
serial: 0x65a91e7d
manufacturing date: 2019/9
====================SMART info====================
Device model: longsys MPS
Version: 2
Size of SMART: 60
Initial Bad Block Count: 25
Increase Bad Block Count: 0
Amount of Data Written: 16918.767699 GB 
Number of Reserved Blocks: 8
Total Erase Count:: 202750
Total Sector Count: 493056000
Remaining Lifetime: 93%
Remaining data write amount: 744767 GB
Flash Endurance: 3000 P/E
Amount of Data Written to Flash: 53982 GB
```

### Sandisk Industrial

```
/dev/mmcblk0 device bus: "/sys/bus/mmc/devices/mmc0:aaaa"
====================CID info====================
manufacturer: SanDisk
OEM ID: SD
product: SA08G
hwrev: 8
fwrev: 0
serial: 0xc5773cd0
manufacturing date: 2018/10
====================SMART info====================
Device model: Sandisk
Manufacture date(YYMMDD): 181025
Health Status Percent Used: 1
Feature Revision: 0x1f
Generation Identifier: 5
Programmable Product String: SanDisk                         
Power-on times: 59
Product number: C1f30p       SDSA08G
SN: C5 77 3C D0 01 2A
```

### Sandisk UHS-II(maybe fake?)

```
/dev/mmcblk0 device bus: "/sys/bus/mmc/devices/mmc0:0007"
====================CID info====================
manufacturer: SanDisk [0x03]
OEM ID: PT
product: SP2BN
hwrev: 3
fwrev: 0
serial: 0xdad8524a
manufacturing date: 2016/10
====================SMART info====================
Device model: Apacer
Flash Id: [0x98,0x3a,0x95,0x93,0x7a,0x51,0x08,0x04,0x04]
IC Version: [0x1f,0x64]
FW Version: [36,05]
CE Number: 4
Bad Block Replace Maximum:: 46
Spare Block Count: 46
Initial Bad Block Count: 134
Good Block Rate: 99.22%
Total Erase Count: 10121
Remain Life: 99.84%
Average Erase Count: 5
Minimum Erase Count: 1
Maximum Erase Count: 13
Power Up Count: 50
Abnormal Power Off Count: 0
Total Refresh Count: 0
Product Marker: 
Later Bad Block Count: 0
```


## Thanks

sdinsight is inspired by following projects, big thanks to them!

https://www.mydigit.cn/thread-349896-1-1.html

https://www.mydigit.cn/thread-335366-1-1.html

https://github.com/Ognian/sdmon

https://github.com/CTCaer/hekate

https://github.com/transcend-information/RaspberryPi_NVIDIAJetson-SDcard-SMARTQuery

https://git.kernel.org/pub/scm/utils/mmc/mmc-utils.git

特别感谢 `hhdjz13813` 在开发过程中提供的帮助。