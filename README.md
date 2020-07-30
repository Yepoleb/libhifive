# libhifive

Alternative SDK for the SiFive HiFive1 Rev B. Started because I hated working with the official Freedom Platform SDK. Still missing some essential features like interrupt handling.

## Structure

* definitions/: Hardware definition files
* generated/: Generated SDK
* buildsdk.py: Build script
* template.h: Header template
* device_extra/: Extra code to be included in device headers
* extra/: Base skeleton files for the SDK
* extra/bin/hifive: Script to launch the uploader and debugger
* extra/example: Example projects

## Prerequisites

For compiling projects:

* [riscv-gnu-toolchain](https://github.com/riscv/riscv-gnu-toolchain) in `/opt/riscv`

For building the SDK:

* Python >= 3.5
* Jinja2

## Building

### SDK

```sh
python3 buildsdk.py
```

### Examples (or your own projects)

Run this code from the root of the generated SDK, not the project root.

```sh
cd example/helloworld
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../../../RISCVToolchain.cmake -DLIBHIFIVE_DIR=../../..
make
```

### Documentation

It's just a list of file contents, nothing has proper docstrings yet.

```sh
doxygen
xdg-open docs/index.html
```

## Uploading

```sh
bin/hifive upload <project>.hex
```

## Debugging

Make sure to recompile both the library and your project with `-DCMAKE_BUILD_TYPE=Debug` first.

```sh
bin/hifive debug <project>.elf
```

## What is mapped?

| Address    | Name        | Mapped In |
| ---------- | ----------- | --------- |
| 0x02000000 | CLINT       | skipped   |
| 0x0C000000 | PLIC        | skipped   |
| 0x10000000 | AON         | aon.xml   |
| 0x10008000 | PRCI        | prci.xml  |
| 0x10010000 | OTP Control |           |
| 0x10012000 | GPIO        | gpio.xml  |
| 0x10013000 | UART 0      | uart.xml  |
| 0x10014000 | QSPI 0      | spi.xml   |
| 0x10015000 | PWM 0       | pwm.xml   |
| 0x10016000 | I2C 0       | i2c.xml   |
| 0x10023000 | UART 1      | uart.xml  |
| 0x10024000 | SPI 1       | spi.xml   |
| 0x10025000 | PWM 1       | pwm.xml   |
| 0x10034000 | SPI 2       | spi.xml   |
| 0x10035000 | PWM 2       | pwm.xml   |

## License

[MIT](LICENSE)
