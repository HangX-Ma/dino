#include "lgfx/v1/platforms/sdl/Panel_sdl.hpp"
#include "dino/state.h"

#if defined(SDL_h_)

static dino::GameState game;

// clang-format off
__attribute__((weak))
int userFunc(bool *running)
{
    game.setup();
    do {
        game.loop();
    } while (*running);
    return 0;
}

int main(int /*unused*/, char ** /*unused*/) { return lgfx::Panel_sdl::main(userFunc); }
// clang-format on

#endif
