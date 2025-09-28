<img src="app/src/main/assets/icon.bmp" width="256" alt="logo">

# 🎮 SENSE: The Game

## 🧩 Description

**"SENSE: The Game"** is a unique philosophical game inspired by the myth of Sisyphus, where you are tasked not just with pushing a boulder, but also with seeking answers to life's most profound questions. This game explores the concept of the meaning of life through the lens of absurdity, repetitive actions, and endless cycles.

### 🗿 Story and Gameplay

You take on the role of Sisyphus, who, according to the myth, is condemned to push a boulder to the top of a mountain, only for it to roll back down every time. In the game, each step and each attempt highlight the absurdity of existence and the constant obstacles on the path to achieving your goal.

Each step on the mountain is not just a challenge in the game, but a challenge to yourself.

# Project Compilation Guide

## Windows

### 64-bit build

``` bat
cmake -S app/src/main -B build64 -G "Visual Studio 17 2022" ^
  -A x64 ^
  -DBUILD_SHARED_LIBS=ON ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=build/release64 ^
  -D CMAKE_C_FLAGS="/D_WIN32_WINNT=0x0601 /DNTDDI_VERSION=0x06010000" ^
  -D CMAKE_CXX_FLAGS="/D_WIN32_WINNT=0x0601 /DNTDDI_VERSION=0x06010000"

cmake --build build64 --config Release
cmake --install build64
```

### 32-bit build

``` bat
cmake -S app/src/main -B build32 -G "Visual Studio 17 2022" ^
  -A Win32 ^
  -DBUILD_SHARED_LIBS=ON ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=build/release32 ^
  -D CMAKE_C_FLAGS="/D_WIN32_WINNT=0x0601 /DNTDDI_VERSION=0x06010000" ^
  -D CMAKE_CXX_FLAGS="/D_WIN32_WINNT=0x0601 /DNTDDI_VERSION=0x06010000"

cmake --build build32 --config Release
cmake --install build32
```

------------------------------------------------------------------------

## Linux

### 64-bit build

``` bash
cmake -S app/src/main -B build64   -DCMAKE_BUILD_TYPE=Release   -DBUILD_SHARED_LIBS=ON   -DCMAKE_INSTALL_PREFIX="/home/release64"   -DCMAKE_C_FLAGS="-m64"   -DCMAKE_CXX_FLAGS="-m64"

cmake --build build64 --config Release
cmake --install build64
```

### 32-bit build

``` bash
cmake -S app/src/main -B build32   -DCMAKE_BUILD_TYPE=Release   -DBUILD_SHARED_LIBS=ON   -DCMAKE_INSTALL_PREFIX="/home/release32"   -DCMAKE_C_FLAGS="-m32"   -DCMAKE_CXX_FLAGS="-m32"

cmake --build build32 --config Release
cmake --install build32
```

------------------------------------------------------------------------

## Android

For building on **Android**, please use **Android Studio**.

## 🧑‍💻 Authors

- [IPOleksenko](https://github.com/IPOleksenko) (owner) — Developer and creator of the idea.
- [Vortm4x](https://github.com/Vortm4x) — Developer.
- makarenk0_vlad — 2D artist.

## 📜 License

This project is licensed under the [MIT License][license].

[license]: ./LICENSE

---

> _"One must imagine Sisyphus happy."_ — Albert Camus
