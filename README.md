<div align="center">

[![C++](https://img.shields.io/badge/Language-C%2B%2B-%23f34b7d.svg?style=plastic)](https://en.wikipedia.org/wiki/C%2B%2B)
[![LOL](https://img.shields.io/badge/Game-League%20of%20Legends-445fa5.svg?style=plastic)](https://na.leagueoflegends.com)
[![Windows](https://img.shields.io/badge/Platform-Windows-0078d7.svg?style=plastic)](https://en.wikipedia.org/wiki/Microsoft_Windows)
[![x86](https://img.shields.io/badge/Arch-x86-red.svg?style=plastic)](https://en.wikipedia.org/wiki/X86)
[![License](https://img.shields.io/github/license/Emre37destan/KralSkin-EN.svg?style=plastic)](LICENSE)
[![Issues](https://img.shields.io/github/issues/Emre37destan/KralSkin-EN.svg?style=plastic)](https://github.com/Emre37destan/KralSkin-EN/issues)
![Windows](https://github.com/Emre37destan/KralSkin-EN/workflows/Windows/badge.svg?branch=main&event=push)
# **KralSkin-EN**
<img src="https://raw.githubusercontent.com/Emre37destan/KralSkin-EN/main/KralSkin-EN.png">
</div>

`KralSkin-EN` is a built-in skin changer for League of Legends.
- Change the appearance of your champion and other champions in the game.
- Automatic skins database update.
- Support for spectator mode.
- Change skins anytime and unlimited times in a single game.
- Supports all Popular languages ​​in the world.
- In-game configuration with <a href="https://github.com/ocornut/imgui">ImGui</a>.
- <a href="https://github.com/nlohmann/json">JSON</a> based configuration save and load

# Building
1. Clone the source with `git clone --recursive https://github.com/Emre37destan/KralSkin-ENN.git`
2. Build in Visual Studio 2017/19 with "Your Region" configuration

# Use
1. Use `KralSkin_Injektor` or inject the resulting DLL into the game yourself.
- *Administrator* privilege may be required if injection fails.
- League client may crash if injected before entering the arena.
- A workaround is not to inject until you are in the arena (you need to be quick to not break the game).
2. Press <kbd>Add</kbd> to bring up the menu.
3. Choose skins for yourself, teammates, enemies, wards.

# Other optimizations
If your CPU supports the AVX / AVX2 / AVX-512 instruction set, you can enable it in the project settings. This should result in more performant code optimized for your CPU.

# Credit
This program is <a href="https://github.com/R3nzTheCodeGOD">B3akers</a>/<a href="https://github.com/R3nzTheCodeGOD/R3nzSkin">LeagueSkinChanger</a> It is an improved and updated version.
