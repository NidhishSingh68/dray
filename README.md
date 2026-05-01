
# STM32F4 C++ Meson Template

This repository is a minimal template for building **C++ firmware for STM32F4 microcontrollers** using **Meson**.

It uses **libopencm3** as a Meson subproject, so the dependency is fetched and built automatically. The main.cpp in src/ is a simple blinky project.

---

## Setup

Clone the repository and configure the build using the provided cross file:

```sh
git clone https://github.com/NidhishSingh68/STM32F4-template-for-Cpp-with-meson.git
cd STM32F4-template-for-Cpp-with-meson
meson setup build --cross-file cross_file.txt
```

## Compile 

```sh
meson compile -C build
```

## Flash

```sh
meson compile flash -C build/
```

## Optional

Strip the binary to reduce size:
```sh
arm-none-eabi-strip ./build/helloWorld
```


