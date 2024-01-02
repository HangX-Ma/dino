#include "lgfx/v1/platforms/sdl/Panel_sdl.hpp"
#if defined(SDL_h_)

void setup();
void loop();

// clang-format off
__attribute__((weak))
int userFunc(bool *running)
{
    setup();
    do {
        loop();
    }
    while (*running);
    return 0;
}

int main(int /*unused*/, char ** /*unused*/) { return lgfx::Panel_sdl::main(userFunc); }
// clang-format on

#endif