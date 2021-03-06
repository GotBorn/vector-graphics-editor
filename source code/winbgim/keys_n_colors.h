#if !defined(__KEYS_N_COLORS)
#define __KEYS_N_COLORS

enum _colors {
  BLACK,          /* dark colors */
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  LIGHTGRAY,
  DARKGRAY,       /* light colors */
  LIGHTBLUE,
  LIGHTGREEN,
  LIGHTCYAN,
  LIGHTRED,
  LIGHTMAGENTA,
  YELLOW,
  WHITE
};

enum _keys { KEY_F1=59, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM_LOCK, KEY_SCROLL_LOCK,
        KEY_HOME, KEY_UP, KEY_PGUP, KEY_LEFT, KEY_CENTER, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PGDN, KEY_INSERT, KEY_DELETE,
        KEY_F11=87, KEY_F12, KEY_ESC=27, KEY_TAB=9, KEY_BACKSPACE=8, KEY_ENTER=13, KEY_CTRL=29, KEY_SHIFT=42, KEY_ALT=59};
#endif
