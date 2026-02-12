# Germen Pulchrum

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)
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
translations, plus carefully selected fonts:
* **IBM Plex Sans** for crisp, scalable text in multiple languages;
* **Noto Color Emoji** for vibrant emoji support;
* **BabelStone Flags** for country flag icons.

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

**📦 Single-file deployment**
All resources — fonts, translations, and assets — are embedded in the executable. One file, zero dependencies, maximum
portability.

The template also includes theme support, giving you a complete foundation for building polished desktop applications.

## Why "Germen Pulchrum"?

The name *Germen Pulchrum* comes from Latin and literally means "beautiful seed" or "quality seed". It perfectly
represents the project's philosophy: a fertile starting point (*germen*) with careful attention to quality (*pulchrum*)
from which modern desktop applications can grow.

Just as a quality seed produces excellent fruit, this template provides a solid foundation for developing elegant and
functional graphical interfaces. The attention to user experience quality — from internationalization to carefully
selected fonts, from optimized rendering to interface design — is built in from the very beginning.

In everyday use, the project is simply called **Germen**.

## Requirements

### Windows

* 4 bit Windows 10 or 11;
* Visual Studio 2022 with tool-set v143;
* CMake 4.x;
* Conan 2.x;
* SDK Vulkan 1.4.309.0:
    * note: the program use Vulkan 1.1 so probably an oldest SDK version will also works;
* video card and drivers compatible with Vulkan 1.1.

### Linux

* 64 bit Debian 13 or equivalent distribution;
* CMake 4.x;
* Conan 2.x;
* GCC 12.x, 13.x or 14.x;
* Ninja build system if you use GCC 12 or GCC 13;
* make if you use GCC 14;
* SDK Vulkan 1.4.309.0 (packages libvulkan-dev and vulkan-utility-libraries-dev):
    * note: the program use Vulkan 1.1 so probably an oldest SDK version will also works;
* video card and drivers compatible with Vulkan 1.1.

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

## Project structure

```
Germen/
├── CMake/               # CMake scripts and configurations
├── Germen Pulchrum/     # Main application source code
├── Profili Conan/       # Conan configuration profiles
├── Strumenti/           # Utilities and support tools
├── Terze parti/         # Third-party libraries, assets and their configurations
├── CMakeLists.txt       # Main CMake file
├── CMakePresets.json    # CMake configuration presets
├── conanfile.py         # Conan dependency management
├── LICENSE              # MIT License
└── README.en.md         # This file (English readme)
├── README.md            # Italian readme
```

## Contributing

Contributions are welcome! If you'd like to contribute to Germen Pulchrum:

1. fork the project;
2. create a feature branch (`git checkout -b feature/NewFeature`);
3. commit your changes (`git commit -m 'Add NewFeature'`);
4. push to GitHub (`git push origin feature/NewFeature`);
5. ppen a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

Germen Pulchrum is built on top of these excellent open source libraries:

- [Dear ImGui](https://github.com/ocornut/imgui): immediate Mode GUI;
- [SDL3](https://www.libsdl.org/): Simple DirectMedia Layer;
- [Vulkan](https://www.vulkan.org/): cross-platform graphics API;
- [Boost.Locale](https://www.boost.org/): internationalization library;
- [IBM Plex](https://github.com/IBM/plex): IBM Plex font family;
- [Noto Color Emoji](https://github.com/googlefonts/noto-emoji): Google's emoji font;
- [BabelStone Flags](https://www.babelstone.co.uk/Fonts/): special flags symbols font.

The complete list of all the libraries used are inside the [third parties readme](Terze%20parti/Readme.en.md).
