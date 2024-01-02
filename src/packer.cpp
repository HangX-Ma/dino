#define LGFX_USE_V1
#include <LGFX_AUTODETECT.hpp>
#include <LovyanGFX.hpp>
#include <random>
#include "monitor.h"

static LGFX lcd(240, 240);

static LGFX_Sprite *canvas;

#define PIN_LEFT 39
#define PIN_DOWN 38
#define PIN_RIGHT 37
#define PIN_UP 36

std::random_device rd;
std::mt19937 gen(rd());
int gameRandom(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

static Monitor monitor;

void setup()
{
    lcd.init();
    canvas = new LGFX_Sprite(&lcd);
    canvas->createSprite(lcd.width(), lcd.height());
}

void loop()
{
    canvas->fillSmoothCircle(gameRandom(0, canvas->width()), gameRandom(0, canvas->height()),
                             gameRandom(1, 24), gameRandom(TFT_BLACK, TFT_WHITE));

    canvas->pushSprite(0, 0);

    // lgfx::delay(5);
    monitor.update();
}
