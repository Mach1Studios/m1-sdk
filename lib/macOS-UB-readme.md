# Workaround solutions for compiling Fat macOS Universal Binary for Mach1 Spatial System plugins

## Steps
- after compiling xcode toolchain open the xcode project
- Edit Scheme to be release
- On `PROJECT` set `Architectures` to `Standard Architectures`
- On `PROJECT` set `Base SDK` to `macOS`
- on `PROJECT` set `Build Active Architeure Only` to `No`
- Change Scheme to `install` and build
- Copy new release bins

## TODO
- automate this with a new polly toolchain/cmake command
```
set(ENV{ARCHS_STANDARD})
set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD))
set(XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
```