# 📝 Cardputer Notes App (Current Version)

A lightweight Notes app for the **M5Stack Cardputer (ESP32‑S3)**.  
This version includes a working menu system, note creation with editing features, note storage in memory, and a notes viewer.

---
![Cardputer](photos/notes1.jpeg)

## ✅ Features (Current)
- **Menu** with 3 options:  
  1. **New Note**  
  2. **View Notes**  
  3. **About**
- **New Note**
  - Type text with `M5Canvas`
  - **Backspace/Delete support** (remove characters before cursor)
  - **Left / Right arrows** move the cursor
  - **Enter** saves the note (stored in RAM, up to 20 notes)
  - **` (backtick)** returns to menu
  - Cursor visibly blinks for feedback
- **View Notes**
  - Shows saved notes in a scrollable window (8 notes at a time)
  - Navigation with sliding window logic
- **About screen**
  - Displays version and author info

---

## ⚠️ Limitations (Current)
- Notes are stored in RAM → lost on reset (no persistent storage yet)
- No multi-line editing yet
- Scrolling in note editor not implemented

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

## 🚀 Future Plans
- Add **persistent storage** (SPIFFS/SD card support)
- Improve **View Notes** navigation (scrolling, delete notes)
- Enable **multi-line note editing**
- Enhance **cursor & text rendering** for smoother UX


