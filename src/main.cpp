#include "M5Cardputer.h"
#include "M5GFX.h"

// Uygulama durumlarını (state) tanımlıyoruz
enum AppState {
    MENU,
    NEW_NOTE,
    VIEW_NOTES,
    ABOUT
};

// Şu anki state'i tutan değişken
AppState currentState = MENU;

void drawMenu() {
    // Clear screen
    M5Cardputer.Display.fillScreen(BLACK);

    // Purple border
    M5Cardputer.Display.drawRect(0, 0,
        M5Cardputer.Display.width(),
        M5Cardputer.Display.height(),
        PURPLE);

    // Küçük default font
    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextSize(2);

    // Print the main menu
    M5Cardputer.Display.setCursor(5, 5);
    M5Cardputer.Display.println("Notes App\n");
    M5Cardputer.Display.println("1. New Note");
    M5Cardputer.Display.println("2. View Notes");
    M5Cardputer.Display.println("3. About");
}

void drawAbout() {
    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setCursor(10, 40);
    M5Cardputer.Display.println("Notes App v1.0");
    M5Cardputer.Display.println("by Ulas");

    // Not: ileride geri dönüş için bir tuş da ekleyeceğiz
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);

    // Başlangıçta menüyü çiz
    drawMenu();
}

void loop() {
    M5Cardputer.update();

    switch (currentState) {
        case MENU: {
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == '1') {
                        currentState = NEW_NOTE;  // Şimdilik sadece state değişiyor
                        // ileride drawNewNote() yapacağız
                    } else if (k == '2') {
                        currentState = VIEW_NOTES;
                        // ileride drawViewNotes() yapacağız
                    } else if (k == '3') {
                        currentState = ABOUT;
                        drawAbout();
                    }
                }
            }
            break;
        }

        case NEW_NOTE: {
            // Geçici mesaj
            M5Cardputer.Display.fillScreen(BLACK);
            M5Cardputer.Display.setCursor(10, 40);
            M5Cardputer.Display.println("New Note screen (TODO)");
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == 96){
                      drawMenu();
                      currentState = MENU;
                    }
                }
            }
            // ileride buraya input sistemi gelecek
            break;
        }

        case VIEW_NOTES: {
            // Geçici mesaj
            M5Cardputer.Display.fillScreen(BLACK);
            M5Cardputer.Display.setCursor(10, 40);
            M5Cardputer.Display.println("View Notes screen (TODO)");
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == 96){
                      drawMenu();
                      currentState = MENU;
                    }
                }
            }
            break;
        }

        case ABOUT: {
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == 96){
                      drawMenu();
                      currentState = MENU;
                    }
                }
            }
            break;
        }
    }
}