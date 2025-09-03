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
    M5Cardputer.Display.println("Notes App v1.1");
    M5Cardputer.Display.println("by Ulas");
}

void canvasSetup(){
  notecanvas.createSprite(M5Cardputer.Display.width()-10, 
                          M5Cardputer.Display.height()-20);

  notecanvas.setFont(&fonts::Font0);
  notecanvas.setTextSize(2);
  notecanvas.setTextScroll(true);
}

void drawNoteWithCursor() {
    notecanvas.fillScreen(BLACK);

    notecanvas.setFont(&fonts::Font0);
    notecanvas.setTextSize(2);

    int startX = 0;
    int startY = 0;

    String header = "New Note (press Enter to save):";
    notecanvas.setCursor(startX, startY);
    notecanvas.setTextColor(WHITE, BLACK);
    notecanvas.print(header);
    notecanvas.print(currentNote);

    // -----------------------------
    // Monospace cursor calculation
    // -----------------------------
    // Character width and how many character by line
    int charWidth = notecanvas.textWidth("A");
    int fontHeight = notecanvas.fontHeight();
    int lineWidth = notecanvas.width() - 2 * startX;
    int charsPerLine = lineWidth / charWidth;

    
    int totalChars = (header.length() + cursorPosition);

    int lineNumber = totalChars / charsPerLine;
    int lineOffset = totalChars % charsPerLine;

    int cursorX = startX + lineOffset * charWidth;
    int cursorY = startY + lineNumber * fontHeight;

    // Draw
    notecanvas.drawLine(cursorX, cursorY, cursorX, cursorY + fontHeight, PURPLE);
    notecanvas.pushSprite(0, 0);
}













void setup() {
    Serial.begin(115200);
    
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
                    if (k == '1') {
                        currentState = NEW_NOTE; 
                        M5Cardputer.Display.fillScreen(BLACK); 
                        drawNoteWithCursor();
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
            
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();

                if (keys.enter) {
                    if (currentNote != ""){   
                        if (noteCount < 20) {
                            notes[noteCount++] = currentNote;
                        }
                        currentNote = "";
                        cursorPosition = 0;
                        drawMenu();
                        currentState = MENU;
                    }
                }
                else if (keys.del && currentNote.length() > 0) {
                    currentNote.remove(currentNote.length() - 1);
                    if (cursorPosition > 0) cursorPosition--;
                    drawNoteWithCursor();
                }
                else {
                    for (auto k : keys.word) {
                        if (k == 96){ // Escape button
                            drawMenu();
                            currentState = MENU;
                        }
                        else if (k == 44 && cursorPosition > 0){ // left arrow
                            cursorPosition--;
                            drawNoteWithCursor();
                        }
                        else if (k == 47 && (cursorPosition < currentNote.length())){ // right arrow
                            cursorPosition++;
                            drawNoteWithCursor();
                        }
                        else {
                            String left  = currentNote.substring(0, cursorPosition);
                            String right = currentNote.substring(cursorPosition);
                            currentNote = left + (char)k + right;
                            cursorPosition++;
                            drawNoteWithCursor();
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
            for(int i=0; i<noteCount; i++){
                notecanvas.setCursor(10, 70 + i*20);
                notecanvas.print(String(i+1) + ". " + notes[i]);
            }
            notecanvas.pushSprite(0,0);

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
