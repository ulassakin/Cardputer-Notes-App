#include "M5Cardputer.h"
#include "M5GFX.h"
#include <SPI.h>
#include <SD.h>

// ---- SD pin config (from Cardputer SD example) ----
#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12

// ---- App states ----
enum AppState {
    MENU,
    NEW_NOTE,
    VIEW_NOTES,
    EDIT_NOTE,
    ABOUT
};

// ---- App data ----
String notes[20];   // max 20 notes
int noteCount = 0;
String currentNote = "";
int cursorPosition = 0;

// List window
int startIndex = 0;
int selectedIndex = 0;
int windowSize = 8;

// Cursor blinking
bool cursorVisible = true;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 500; // ms

// Current state
AppState currentState = MENU;

// Create the Canvas
M5Canvas notecanvas(&M5Cardputer.Display);

// ---------- UI helpers ----------
void drawMenu() {
    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.drawRect(0, 0,
        M5Cardputer.Display.width(),
        M5Cardputer.Display.height(),
        PURPLE);

    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextSize(2);
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
    M5Cardputer.Display.println("Notes App v1.3");
    M5Cardputer.Display.println("by Ulas");
}

void canvasSetup(){
  notecanvas.createSprite(M5Cardputer.Display.width(), 
                          M5Cardputer.Display.height());
  notecanvas.setFont(&fonts::Font0);
  notecanvas.setTextSize(2);
  notecanvas.setTextScroll(true);
}
//Drawing and placing the cursor
void drawNoteWithCursor() {
    notecanvas.fillScreen(BLACK);
    notecanvas.setFont(&fonts::Font0);
    notecanvas.setTextSize(2);

    const int startX = 0;
    const int startY = 0;

    String header = (currentState == NEW_NOTE) 
                  ? "New Note (press Enter to save):"
                  : "Edit Note (press Enter to save):";

    notecanvas.setCursor(startX, startY);
    notecanvas.setTextColor(WHITE, BLACK);
    notecanvas.print(header);
    notecanvas.print(currentNote);

    // Monospace cursor calculation
    int charWidth = notecanvas.textWidth("A");
    int fontHeight = notecanvas.fontHeight();
    int lineWidth = notecanvas.width();
    int charsPerLine = lineWidth / charWidth;

    int totalChars = header.length() + cursorPosition;
    int lineNumber = totalChars / charsPerLine;
    int lineOffset = totalChars % charsPerLine;

    int cursorX;
    int cursorY;

    if (lineNumber >= 8 ){
        cursorX = lineOffset * charWidth;
        cursorY = 7 * fontHeight + 5;
    } else {
        cursorX = startX + lineOffset * charWidth;
        cursorY = startY + lineNumber * fontHeight;
    }

    if (cursorVisible) {
        notecanvas.drawLine(cursorX, cursorY, cursorX, cursorY + fontHeight, WHITE);
    }
    notecanvas.pushSprite(0, 0);
}

// ---------- SD helpers ----------
static String notePathForIndex(int idx1based) {
    return "/note" + String(idx1based) + ".txt";
}

static void writeTextFile(const String& path, const String& content) {
    SD.remove(path); // ensure truncate
    File f = SD.open(path, FILE_WRITE);
    if (!f) return;
    f.print(content);
    f.close();
}

void loadNotesFromSD() {
    noteCount = 0;
    for (int i = 1; i <= 20; ++i) {
        String filename = notePathForIndex(i);
        File file = SD.open(filename);
        if (file) {
            String content = "";
            while (file.available()) {
                char c = file.read();
                // store only printable ASCII to avoid weird control keys
                if (c >= 32 && c <= 126) content += c;
            }
            file.close();
            content.trim();
            if (content.length() > 0) {
                notes[noteCount++] = content;
            }
        }
    }
}

void saveNewNoteToSD(const String& text) {
    if (noteCount >= 20) return;
    int idx = noteCount + 1; // 1-based filename
    String path = notePathForIndex(idx);
    writeTextFile(path, text);
}

void saveEditedNoteToSD(int zeroBasedIndex, const String& text) {
    int idx1 = zeroBasedIndex + 1;
    String path = notePathForIndex(idx1);
    writeTextFile(path, text);
}

void deleteNoteFromSDAndCloseGap(int zeroBasedIndex) {
    // delete target
    SD.remove(notePathForIndex(zeroBasedIndex + 1));

    // shift files down to keep numbering contiguous
    for (int j = zeroBasedIndex + 2; j <= noteCount; ++j) {
        String from = notePathForIndex(j);
        String to   = notePathForIndex(j - 1);
        // remove destination first to avoid rename fail
        SD.remove(to);
        SD.rename(from, to);
    }
}

// ---------- Arduino setup/loop ----------
void setup() {
    Serial.begin(115200);

    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    canvasSetup();
    drawMenu();

    // Init SD same as example: explicit SPI pins + SD.begin with CS & SPI
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
        Serial.println("SD init failed (card missing or init error). Running without SD.");
    } else {
        Serial.println("SD init OK");
        loadNotesFromSD();
    }
}

void loop() {
    M5Cardputer.update();

    if (currentState == NEW_NOTE || currentState == EDIT_NOTE) {
        if (millis() - lastBlink >= blinkInterval) {
            cursorVisible = !cursorVisible;
            lastBlink = millis();
            drawNoteWithCursor();
        }
    }

    switch (currentState) {
        case MENU: {
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == '1') {
                        currentState = NEW_NOTE;
                        M5Cardputer.Display.fillScreen(BLACK);
                        currentNote = "";
                        cursorPosition = 0;
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
                    if (currentNote.length() > 0 && noteCount < 20) {
                        // Save in RAM
                        notes[noteCount++] = currentNote;
                        // Save to SD
                        saveNewNoteToSD(currentNote);
                    }
                    currentNote = "";
                    cursorPosition = 0;
                    drawMenu();
                    currentState = MENU;
                } else if (keys.del && currentNote.length() > 0) {
                    if (cursorPosition > 0) {
                        currentNote.remove(cursorPosition - 1, 1);
                        cursorPosition--;
                    }
                    drawNoteWithCursor();
                } else {
                    for (auto k : keys.word) {
                        if (k == 96) { // ESC
                            drawMenu();
                            currentState = MENU;
                        } else if (k == 44 && cursorPosition > 0) { // left
                            cursorPosition--;
                            drawNoteWithCursor();
                        } else if (k == 47 && (cursorPosition < currentNote.length())) { // right
                            cursorPosition++;
                            drawNoteWithCursor();
                        } else if (k >= 32 && k <= 126) { // printable
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
            for (int i = 0; i < windowSize; i++) {
                int noteIndex = startIndex + i;
                if (noteIndex < noteCount) {
                    int marginTop = 5;
                    notecanvas.setCursor(0, i * notecanvas.fontHeight() + marginTop);

                    if (selectedIndex == noteIndex) {
                        notecanvas.setTextColor(WHITE, PURPLE);
                        notecanvas.println(notes[noteIndex].substring(0, 15));
                        notecanvas.setTextColor(WHITE, BLACK);
                    } else {
                        notecanvas.setTextColor(WHITE, BLACK);
                        notecanvas.println(notes[noteIndex].substring(0, 15));
                    }
                }
            }
            notecanvas.pushSprite(0, 0);

            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();

                if (keys.enter && noteCount > 0) {
                    currentNote = notes[selectedIndex];
                    currentState = EDIT_NOTE;
                    cursorPosition = currentNote.length();
                    drawNoteWithCursor();
                } else if (keys.del && noteCount > 0) {
                    // Delete from SD and RAM, keep file numbering contiguous
                    deleteNoteFromSDAndCloseGap(selectedIndex);

                    for (int i = selectedIndex; i < noteCount - 1; i++) {
                        notes[i] = notes[i + 1];
                    }
                    noteCount--;
                    if (noteCount < 0) noteCount = 0;
                    if (selectedIndex >= noteCount) selectedIndex = noteCount - 1;
                    if (selectedIndex < 0) selectedIndex = 0;
                } else {
                    for (auto k : keys.word) {
                        if (k == 96) { // ESC
                            drawMenu();
                            currentState = MENU;
                        } else if (k == 46 && selectedIndex < noteCount - 1) { // down
                            selectedIndex++;
                            if (selectedIndex >= startIndex + windowSize) startIndex++;
                        } else if (k == 59 && selectedIndex > 0) { // up
                            selectedIndex--;
                            if (selectedIndex < startIndex) {
                                startIndex--;
                                if (startIndex < 0) startIndex = 0;
                            }
                        }
                    }
                }
            }
            break;
        }

        case EDIT_NOTE: {
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();

                if (keys.enter) {
                    notes[selectedIndex] = currentNote;
                    // Overwrite file on SD
                    saveEditedNoteToSD(selectedIndex, currentNote);

                    cursorPosition = 0;
                    drawMenu();
                    currentState = MENU;
                } else if (keys.del && currentNote.length() > 0) {
                    if (cursorPosition > 0) {
                        currentNote.remove(cursorPosition - 1, 1);
                        cursorPosition--;
                    }
                    drawNoteWithCursor();
                } else {
                    for (auto k : keys.word) {
                        if (k == 96) { // ESC
                            drawMenu();
                            currentState = MENU;
                        } else if (k == 44 && cursorPosition > 0) { // left
                            cursorPosition--;
                            drawNoteWithCursor();
                        } else if (k == 47 && (cursorPosition < currentNote.length())) { // right
                            cursorPosition++;
                            drawNoteWithCursor();
                        } else if (k >= 32 && k <= 126) { // printable
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

        case ABOUT: {
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto keys = M5Cardputer.Keyboard.keysState();
                for (auto k : keys.word) {
                    if (k == 96){ // ESC
                        drawMenu();
                        currentState = MENU;
                    }
                }
            }
            break;
        }
    }
}

