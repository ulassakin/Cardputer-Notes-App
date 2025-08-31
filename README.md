# 📝 Cardputer Notes App (WIP)

A tiny **Notes** app for the **M5Stack Cardputer (ESP32‑S3)**, built step‑by‑step with a simple **state machine** and a **canvas-based UI**. This is an in‑progress README describing the current milestone.

---

## ✅ Current Features
- **Main Menu** (state machine)
  - `1` → **New Note** (type, press **Enter** to save in RAM)
  - `2` → **View Notes** (placeholder screen for now)
  - `3` → **About** (version screen)
- **New Note** screen
  - Smooth typing using `M5Canvas` (`setTextScroll(true)`)
  - **Enter**: saves to an in‑memory list (`notes[20]`)
  - **\`` (backtick)**: return to Menu
- Basic structure ready for future SD‑card persistence

> Note: ESC/arrow keys can be handled later; mapping depends on the keyboard scan output. 

---

## 🧭 Roadmap (Next Steps)
- [ ] **Backspace** support (delete last char + redraw input line)
- [ ] **View Notes** list (scroll with ↑/↓, open & read)
- [ ] **Persist notes on SD card** (save each note as `note-N.txt` or a single JSON/TXT index)
- [ ] **Delete/Edit note**
- [ ] **Settings** (font size, theme)
- [ ] Refactor into multiple files when code grows (e.g., `menu.cpp`, `notes.cpp`, `storage.cpp`)

---

## 🗂️ Project Structure (PlatformIO)
Typical layout:
```
.
├─ src/
│  └─ main.cpp        # your current code
├─ include/           # (future) headers
├─ lib/               # (future) custom libs
├─ platformio.ini
└─ README.md
```

**Tip:** Put your code in `src/main.cpp`. Keep single‑file until complexity requires splitting.

---

## 🔧 Build & Upload (PlatformIO)
1. Open the folder in **VS Code** with **PlatformIO**.
2. Ensure board env matches Cardputer’s core (e.g. `m5stack-stamp-s3`).  
3. (Optional) For easier flashing, try CDC bootloader:
   ```ini
   ; platformio.ini
   [env:m5stack-stamp-s3]
   platform = espressif32
   board = m5stack-stamp-s3
   framework = arduino
   monitor_speed = 115200
   upload_protocol = usb
   ```
4. **Upload**: click the ► “Upload” button in the PlatformIO toolbar.  
5. **Serial Monitor** (optional): PlatformIO → Monitor (115200).

> If macOS complains about the serial port after upload, unplug/replug USB or set `upload_port` manually (e.g., `/dev/cu.usbmodemXXXX`).

---

## 🎮 Controls (Current)
- **Menu**: press `1`, `2`, `3`
- **New Note**:
  - Type freely (rendered on `M5Canvas` to avoid flicker)
  - **Enter**: save note (RAM)
  - **\`` (backtick)**: cancel & return to Menu

---

## ⚠️ Limitations (for now)
- No **Backspace** yet
- **View Notes** not implemented
- Notes are **not persistent** (lost on restart) until SD support is added

---

## 📄 License
MIT (suggested). Add a `LICENSE` file when you’re ready.

---

## 🙌 Credits
Built with **M5Cardputer**, **M5GFX**, and **PlatformIO**. 
