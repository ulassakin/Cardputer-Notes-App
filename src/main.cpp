#include "M5Cardputer.h"
#include "M5GFX.h"


// Define app states
enum AppState {
    MENU,
    NEW_NOTE,
    VIEW_NOTES,
    ABOUT
};

//Define the variables

String notes[20];   // max 20 notes for now
int noteCount = 0;
String currentNote = "";
int cursorPosition = 0;

// Current state variable
AppState currentState = MENU;

M5Canvas notecanvas(&M5Cardputer.Display); //Create the canvas

void drawMenu() {
    // Clear screen
    M5Cardputer.Display.fillScreen(BLACK);

    // Purple border
    M5Cardputer.Display.drawRect(0, 0,
        M5Cardputer.Display.width(),
        M5Cardputer.Display.height(),
        PURPLE);

    // Small default font
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

    // Draw the main menu at the beginning
    drawMenu();
}

void loop() {
    M5Cardputer.update();

    switch (currentState) {
        case MENU: {
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    Serial.println(keys.word.size());
                    if (k == '1') {
                        currentState = NEW_NOTE;  
                    
                    } else if (k == '2') {
                        currentState = VIEW_NOTES;
                        
                    } else if (k == '3') {
                        currentState = ABOUT;
                        drawAbout();
                    }
                }
            }
            break;
        }

        case NEW_NOTE: {
    
            if (currentNote == "") {
              
              notecanvas.fillScreen(BLACK);
              notecanvas.setCursor(5, 5);
              notecanvas.print("New Note (press Enter to save): ");
              notecanvas.pushSprite(0,0);
              currentNote = "";
              
            }
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();

                if (keys.enter) {
                    if (currentNote != ""){   
                        if (noteCount < 20) {
                            notes[noteCount++] = currentNote;
                        }
                        currentNote = "";
                        drawMenu();
                        currentState = MENU;
                    }
                }

                

                else if (keys.del && currentNote.length() > 0) {
                    currentNote.remove(currentNote.length() - 1);
                    
                    notecanvas.fillScreen(BLACK);
                    notecanvas.setCursor(5, 5);
                    notecanvas.print("New Note (press Enter to save): ");
                    notecanvas.println(currentNote);
                    notecanvas.pushSprite(0,0);
                }


                else{
                    for (auto k : keys.word) {
                        if (k == 96){

                        drawMenu();
                        currentState = MENU;
                        }

                        else if (k == 44 && cursorPosition > 0){
                            cursorPosition--;
                        }
                        else if (k == 47 && (cursorPosition < currentNote.length())){
                            cursorPosition++;
                        }

                        else{
                        
                        String left  = currentNote.substring(0, cursorPosition);
                        String right = currentNote.substring(cursorPosition);
                        currentNote = left + (char)k + right;
                        cursorPosition++;
                        notecanvas.fillScreen(BLACK);
                        notecanvas.setCursor(5, 5);
                        notecanvas.print("New Note (press Enter to save): ");
                        notecanvas.println(currentNote);
                        notecanvas.pushSprite(0,0);

                        }
                    }
                }
            }

            
            
            break;
        }

        case VIEW_NOTES: {
            
            notecanvas.fillScreen(BLACK);
            notecanvas.setCursor(10, 40);
            notecanvas.print("View Notes:");
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