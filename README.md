# libhifive

Alternative SDK for the SiFive HiFive1 Rev B. Started because the official Freedom Platform SDK has one of the worst APIs I have ever seen. Still missing some essential features like interrupt handling.

## Prerequisites

* [riscv-gnu-toolchain](https://github.com/riscv/riscv-gnu-toolchain) in `/opt/riscv`

## Compiling

### Main Library

```sh
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../RISCVToolchain.cmake
make
```

### Examples (or your own projects)

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

## License

[MIT](LICENSE)
