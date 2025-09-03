# 📝 Cardputer Notes App 

A small Notes app for the **M5Stack Cardputer (ESP32‑S3)**.  
Supports creating, viewing, and storing up to 20 notes in memory.

---
<img src="photos/notes.jpeg" alt="Cardputer" width="400" align="left"/>
<br clear="left"/>



## ✅ Current Features
- **Menu** with 3 options (New Note, View Notes, About)
- **New Note** screen
  - Type text smoothly with `M5Canvas`
  - Editable text with cursor navigation (left/right arrows)
  - **Backspace/Delete** support
  - **Enter** saves the note (in memory, max 20)
  - **` (backtick)** returns to menu
- **View Notes** screen
  - Displays all saved notes (scrollable list)
  - **` (backtick)** returns to menu
- **About** screen with version info (v1.0 by Ulas)

---

## ⚠️ Limitations
- Notes are not persistent (lost on reset)
- Limited to 20 notes in RAM

---

## 🗂️ PlatformIO `platformio.ini`
```ini
[env:m5stack-stamp-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
upload_speed = 1500000
build_flags = 
    -DESP32S3
    -DCORE_DEBUG_LEVEL=5
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1
lib_deps = 
    M5Unified=https://github.com/m5stack/M5Unified
    m5stack/M5GFX@^0.2.9
    m5stack/M5Unified@^0.2.7
    m5stack/M5Cardputer@^1.0.3
```

---

## 🎮 Controls
### The cardputer is controlled through the built-in keyboard
- `1` = New Note
- `2` = View Notes 
- `3` = About
- **Enter** = Save note
- **Del** = Backspace/Delete
- **← / → arrows** = Move cursor
- **ESC / ` (backtick)** = Return to menu

---
