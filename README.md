# sdinsight

Just another SD/TF Cards CID & SMART reader.

`/dev/mmcblk0` device is **required** .

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
# default device /dev/mmcblk0 is used
sudo ./sdinsight
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