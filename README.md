# LGFX simulator SDL

**LovyanGFX** PC simulator on linux platform with SDL2. Check [LovyanGFX/CMake_SDL](https://github.com/lovyan03/LovyanGFX/tree/master/examples_for_PC/CMake_SDL) for details.

> CMake recommendation ref: [Modern CMake: Do's and Don'ts](https://cliutils.gitlab.io/modern-cmake/chapters/intro/dodonot.html)

## Prerequisite

- Ubuntu 20.04 or higher
- CMake
- Clang or GNU build toolchain
- SDL2

```bash
sudo apt-get update
sudo apt-get install -y build-essential libsdl2-dev
```

## Usage

```bash
git clone https://github.com/HangX-Ma/LGFX-simulator-SDL.git
cd LGFX-simulator-SDL && git submodule update --init --recursive
cmake -B build
cmake --build build -j$(nproc)
```

## License

MIT License
