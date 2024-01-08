# Dino

This project reimplement the Google Chrome Dino Game based on LovyanGFX driver. You can now run the game on **Linux** platform using SDL2 simulator. If you want to run the game on **Windows**, please check [win](https://github.com/HangX-Ma/dino/tree/win) branch and follow the guide to do cross compile.

**You Dino can get special skill if you reach high score!**

## Dino Game Demo

<div class="dino" align="center">
<img src="./assets/dino.gif" alt="dino game" width=240 />

<font size="2" color="#999"><u>Dino Game</u></font>
</div>

- **Quick Drop:** If you help Dino gets 1000 scores, Dino can obtain the ability to quick drop from the air. (&darr;)
- **Double Jump:** If you help Dino gets 2500 scores, Dino can obtain the ability to make another jump in the air. (&uarr;)

You can use arrow key &uarr; to jump, &darr; to bend over. &rarr; will be used to restart game if your Dino has been dead.

## Dino Game: On day

<div class="dino" align="center">
  <table>
    <tr>
      <td><img src="./assets/dino_game_cactus_day.png" alt="dino game cactus day" width=200 />
      <td><img src="./assets/dino_game_bird_day.png" alt="dino game bird day" width=200 />
      <td><img src="./assets/dino_game_over_day.png" alt="dino game over day" width=200 />
    </tr>
    <tr>
      <td align="center"><font size="2" color="#999"><u>Cactus</u></font></td>
      <td align="center"><font size="2" color="#999"><u>Bird</u></font></td>
      <td align="center"><font size="2" color="#999"><u>Dino Game Over</u></font></td>
    </tr>
  </table>
</div>

## Dino Game: On night

<div class="dino" align="center">
  <table>
    <tr>
      <td><img src="./assets/dino_game_cactus_night.png" alt="dino game cactus night" width=200 />
      <td><img src="./assets/dino_game_bird_night.png" alt="dino game bird night" width=200 />
      <td><img src="./assets/dino_game_over_night.png" alt="dino game over night" width=200 />
    </tr>
    <tr>
      <td align="center"><font size="2" color="#999"><u>Cactus</u></font></td>
      <td align="center"><font size="2" color="#999"><u>Bird</u></font></td>
      <td align="center"><font size="2" color="#999"><u>Dino Game Over</u></font></td>
    </tr>
  </table>
</div>

> This project use some materials from [AquaCoder0010/Dino-Game-SFML](https://github.com/AquaCoder0010/Dino-Game-SFML).

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
git clone https://github.com/HangX-Ma/dino.git
cd dino && git submodule update --init --recursive
cmake -B build
cmake --build build -j$(nproc)
# run the game!
./build/dino
```

## Third Party Library and Tools

- [lovyan03/LovyanGFX](https://github.com/lovyan03/LovyanGFX)
- [gabime/spdlog](https://github.com/gabime/spdlog)
- [riuson/lcd-image-converter](https://github.com/riuson/lcd-image-converter)

## License

MIT License
