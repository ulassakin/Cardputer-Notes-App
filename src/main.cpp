#include "M5Cardputer.h"
#include "M5GFX.h"

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);

    // Küçük default font
    M5Cardputer.Display.setFont(&fonts::Font0);   // en küçük
    M5Cardputer.Display.setTextSize(2);           // biraz büyüt (1-3 arası oynayabilirsin)

    // Clear screen
    M5Cardputer.Display.fillScreen(BLACK);

    // Purple border
    M5Cardputer.Display.drawRect(0, 0,
        M5Cardputer.Display.width(),
        M5Cardputer.Display.height(),
        PURPLE);

    // Print the main menu
    M5Cardputer.Display.setCursor(5, 5);
    M5Cardputer.Display.println("Notes App\n");
    M5Cardputer.Display.println("1. New Note");
    M5Cardputer.Display.println("2. View Notes");
    M5Cardputer.Display.println("3. Exit");
}

void loop() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
        auto keys = M5Cardputer.Keyboard.keysState();
        for (auto k : keys.word) {
            if (k == '1') {
                M5Cardputer.Display.setCursor(10, 120);
                M5Cardputer.Display.println("New Note selected");
            } else if (k == '2') {
                M5Cardputer.Display.setCursor(10, 140);
                M5Cardputer.Display.println("View Notes selected");
            } else if (k == '3') {
                M5Cardputer.Display.setCursor(10, 160);
                M5Cardputer.Display.println("Exit selected");
            }
        }
    }
}
