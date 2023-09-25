# SFSE Plugin template
Yet another Starfield script extender plugin template using my preferred setup

## Initial setup

Set Author and Project Name here
https://github.com/FlayaN/SF_SimplePluginTemplate/blob/main/CMakeLists.txt#L3-L5

## License

By using [this branch](https://github.com/FlayaN/SF_SimplePluginTemplate/tree/main), you agree to comply with [CommonLibSF](https://github.com/Starfield-Reverse-Engineering/CommonLibSF) license, which is [GPL-3.0-or-later](COPYING) WITH [Modding Exception AND GPL-3.0 Linking Exception (with Corresponding Source)](EXCEPTIONS). Specifically, the Modded Code is Starfield (and its variants) and Modding Libraries include [Starfield Script Extender](https://github.com/ianpatt/sfse).  

To put it shortly: when you distribute a binary linked against CommonLibSF, you are obliged to provide access to the source code as well.  

## Build

### Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

```bat
rd /s /q "%~dp0/build"
cmake --preset msvc-win64-vcpkg
cmake --build build --preset msvc-win64-vcpkg-release
```

### Dependencies

- [CommonLibSF](https://github.com/Starfield-Reverse-Engineering/CommonLibSF) (submodule)
- [CLibUtil](https://github.com/powerof3/CLibUtil) (cmake portfile)
