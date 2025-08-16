# CavEX

*Cave Explorer* is a Wii homebrew game with the goal to recreate most of the core survival aspects up until Beta 1.7.3. Any features beyond ~~*will not*~~ cold be added.

---

**Features**
* great performance on Wii (about 60fps)
* 5 chunk render distance currently
* load any beta world save
* nearly all blocks added, except redstone related
* many items from the original
* correct light propagation
* ambient occlusion on blocks

---

**Planned features** *(in no particular order, not complete)*
- [ ] main menu
- [ ] biome colors
- [ ] generation of new chunks
- [x] player physics
- [x] inventory management
- [x] block placement and destruction logic
- [x] (random) block updates
- [x] item actions
- [ ]  real texture pack support
- [ ] Beta 1.7.3 multiplayer support

## Screenshot

![ingame0](docs/ingame0.png)
*(from the PC version)*

## Build instructions

This project is multiplatform, so here you have the specific guide for each platform

### Wii

For the Wii platform you need to install the [devkitPro](https://devkitpro.org/wiki/Getting_Started) Wii/Gamecube environment. Additionally install zlib using pacman of devkitPro.

```bash
dkp-pacman -S wii-dev ppc-zlib
```

To build it just run the following command:
```sh
catnip -T Wii -B build_wii/
```

If you want a debug build, you can run the following command instead:
```sh
catnip -T Wii -B build_wii/ build debug
```

There should then be a .dol file in the root directory that your Wii can run. To copy the game to your `apps/` folder, it needs to look like this:
```
cavex
├── assets
│   ├── terrain.png
│   ├── items.png
│   ├── anim.png
│   ├── default.png
│   ├── gui.png
│   └── gui2.png
├── saves
│   ├── world
│   └── ...
├── boot.dol
├── config.json
├── icon.png
└── meta.xml
```

> [!NOTE]
> If you are using dolphin, you need to enable the synchronization of the SD card with a folder, and there you need to put the files on the __root__
### GNU/Linux

The game can also run on any PC with support for OpenGL 2.0 and played with keyboard and mouse.

Building requires the following additional libraries, which you can install with your system package manager: `zlib`, `glfw3` and `glew`. You can then use CMake and gcc to build. The already existing Makefile is for the Wii platform only and might be removed sometime later.

```bash
mkdir build_pc
cd build_pc
cmake .. -G Ninja
ninja
```

Please also copy the fragment and vertex shaders from `resources/` next to your `assets/` directory.

### Windows (MINGW64)

```sh
pacman -S mingw-w64-x86_64-zlib mingw-w64-x86_64-glfw mingw-w64-x86_64-glew mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-winpthreads mingw-w64-x86_64-pkgconf
```

```sh
cmake -B build_pc -G Ninja
cd build_pc
ninja
```

> [!WARNING]
> Remember that you need to have in the cwd the assets folder and the config.json file!