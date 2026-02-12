# Germen Pulchrum

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)]()
[![Vulkan](https://img.shields.io/badge/Vulkan-1.1-red.svg)](https://www.vulkan.org/)
[![ImGui](https://img.shields.io/badge/ImGui-docking-orange.svg)](https://github.com/ocornut/imgui)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://www.libsdl.org/)
[![Boost](https://img.shields.io/badge/Boost-Locale-purple.svg)](https://www.boost.org/)

[🇮🇹 Italiano](README.md) | [🇬🇧 English](README.en.md)

## English

**Germen Pulchrum** is a high-quality, production-ready template for building cross-platform GUI applications with
Dear ImGui and Vulkan. Designed as a solid starting point for developers at any experience level, it combines technical
excellence with attention to user experience.

### Key features

**🌍 Full internationalization**
Built from the ground up with UTF-8 encoding and Boost.Locale integration. Ships with English (GB) and Italian
translations, plus carefully selected fonts: IBM Plex Sans for crisp, scalable text in multiple languages,
Noto Color Emoji for vibrant emoji support and BabelStone for country flag icons.

**🪟 Flexible window docking**
ImGui's docking system lets you create professional, customizable layouts. Attach tool windows together or snap them to
screen edges, just like Visual Studio or other professional IDEs. Layouts persist automatically between sessions.

**📐 DPI scaling**
Automatic display scaling adapts to your screen's DPI on both Windows and Linux, ensuring crisp rendering on any monitor.
Manual override available in settings for fine-tuned control.

**⚡ Optimized rendering**
VSync-limited rendering (capped at 60 Hz) keeps resource usage low and extends laptop battery life without sacrificing
responsiveness.

**🖥️ Cross-platform**
Runs on Windows 10/11 and Debian 13 (and likely other modern Linux distributions). Compiles with Visual Studio 2022 or
GCC 12/13/14.

**📦 Single-File Deployment**
All resources—fonts, translations, and assets—are embedded in the executable. One file, zero dependencies, maximum
portability.

The template also includes theme support, giving you a complete foundation for building polished desktop applications.

---

## Build requirements

### Windows

* A 64 bit Windows 10 o Windows 11;
* Visual Studio 2022 with tool-set v143;
* CMake 4.x;
* Conan 2.x;
* SDK Vulkan 1.4.309.0:
    * note: the program use Vulkan 1.1 so probably an oldest SDK version will also works.

### Linux

* A 64 bit Linux distribution (it works on Debian);
* CMake 4.x;
* Conan 2.x;
* GCC 12.x, 13.x o 14.x;
* Ninja build system if you use GCC 12 or GCC 13;
* make if you use GCC 14;
* SDK Vulkan 1.4.309.0 (packages libvulkan-dev and vulkan-utility-libraries-dev):
    * note: the program use Vulkan 1.1 so probably an oldest SDK version will also works.

NOTE: Conan could install packages via the system package manager to build some dependencies, in this case Conan will
use sudo to run the package manager and you may therefore need to enter you password in the terminal.

## Test
The program was tested for functionality under the following conditions.

### Windows

* Windows 11;
* NVidia RTX A1000 6GB laptop GPU;
* Intel® Arc™ & Iris® Xe Graphics laptop GPU;

### Linux

* Debian 13 (trixie) through WSL 2 in Windows 11;
* X11 through Xlib;
* llvmpipe (LLVM 19.1.7, 256 bits) software driver;
