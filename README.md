# Dino

This project reimplement the Google Chrome Dino Game based on LovyanGFX driver. You can now run the game on Linx platform using SDL2 simulator.

**You Dino can get special skill if you reach high score (I set it to 3000)!**

## Dino Game Demo

<div class="dino" align="center">
<img src="./assets/dino.gif" alt="dino game" width=240 />

<font size="2" color="#999"><u>Dino Game</u></font>
</div>

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

You can use arrow key &uarr; to jump, &darr; to bend over. &larr; will be used to restart game if your Dino has been dead.

## License

MIT License
