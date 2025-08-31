#include "M5Cardputer.h"
#include "M5GFX.h"

// Uygulama durumlarını (state) tanımlıyoruz
enum AppState {
    MENU,
    NEW_NOTE,
    VIEW_NOTES,
    ABOUT
};

String notes[20];   // max 20 not
int noteCount = 0;
String currentNote = "";

// Şu anki state'i tutan değişken
AppState currentState = MENU;

M5Canvas notecanvas(&M5Cardputer.Display);

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

void canvasSetup(){
  notecanvas.createSprite(M5Cardputer.Display.width()-10, 
                            M5Cardputer.Display.height()-20);

  notecanvas.setFont(&fonts::Font0);
  notecanvas.setTextSize(2);
  notecanvas.setTextScroll(true);
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    canvasSetup();

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
            if (currentNote == "") {
              // İlk girişte ekranı temizle
              notecanvas.fillScreen(BLACK);
              notecanvas.setCursor(5, 5);
              notecanvas.print("New Note (press Enter to save): ");
              notecanvas.pushSprite(0,0);
              currentNote = "";
              
            }
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == 96){
                      drawMenu();
                      currentState = MENU;
                    }

                    else if (keys.enter){
                      if (noteCount < 20){
                        notes[noteCount++] = currentNote;
                      }
                      currentNote = "";
                      drawMenu();
                      currentState = MENU;
                    }

                    else{
                      
                      currentNote += k;
                      notecanvas.print((char)k);
                      notecanvas.pushSprite(0,0);
                    }
                }
            }

            
            // ileride buraya input sistemi gelecek
            break;
        }

        case VIEW_NOTES: {
            // Geçici mesaj
            notecanvas.fillScreen(BLACK);
            notecanvas.setCursor(10, 40);
            notecanvas.print("View Notes screen (TODO)");
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