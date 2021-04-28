#include "win_shortcut_manager.h"

#include <Windows.h>

#include <array>

#include <QtCore/QPointer>
#include <QtCore/QDebug>
#include <QtGui/QWindow>

#include <platform/platform_shortcut_notifier.h>
#include <util/map_access.h>

#include "win_error.h"
#include "win_shortcut_notifier.h"
#include "win_shared_event_window.h"

struct KeyMappingElement {
    /**
     * Qt::Key actually. 0 means Qt::Key_unknown. -1 means that I'll have to look into it later.
     */
    int key;
    Qt::KeyboardModifiers mods;
};

namespace Qt {
    constexpr Qt::Key Key_unimplemented = static_cast<Qt::Key>(Qt::Key_unknown - 1);
}

/**
 * VK_* key mapping table, was derived from qwindowskeymapper.cpp.
 *
 * Unlike Qt, we don't care about how OEM keys are translated on all kinds of different locales.
 * Hotkeys that we're using are always defined in terms of an English keyboard, so this is what we use here.
 */
constexpr std::array<KeyMappingElement, 256> KeyMapping = {{
                                                    // Dec |  Hex | Windows Virtual key
    {0,                     0},                     //   0   0x00
    {0,                     0},                     //   1   0x01   VK_LBUTTON          | Left mouse button
    {0,                     0},                     //   2   0x02   VK_RBUTTON          | Right mouse button
    {Qt::Key_Cancel,        0},                     //   3   0x03   VK_CANCEL           | Control-Break processing
    {0,                     0},                     //   4   0x04   VK_MBUTTON          | Middle mouse button
    {0,                     0},                     //   5   0x05   VK_XBUTTON1         | X1 mouse button
    {0,                     0},                     //   6   0x06   VK_XBUTTON2         | X2 mouse button
    {0,                     0},                     //   7   0x07   -- unassigned --
    {Qt::Key_Backspace,     0},                     //   8   0x08   VK_BACK             | BackSpace key
    {Qt::Key_Tab,           0},                     //   9   0x09   VK_TAB              | Tab key
    {0,                     0},                     //  10   0x0A   -- reserved --
    {0,                     0},                     //  11   0x0B   -- reserved --
    {Qt::Key_Clear,         0},                     //  12   0x0C   VK_CLEAR            | Clear key
    {Qt::Key_Return,        0},                     //  13   0x0D   VK_RETURN           | Enter key                     | Also numpad enter, no way to tell the difference
    {0,                     0},                     //  14   0x0E   -- unassigned --
    {0,                     0},                     //  15   0x0F   -- unassigned --
    {Qt::Key_Shift,         0},                     //  16   0x10   VK_SHIFT            | Shift key
    {Qt::Key_Control,       0},                     //  17   0x11   VK_CONTROL          | Ctrl key
    {Qt::Key_Alt,           0},                     //  18   0x12   VK_MENU             | Alt key
    {Qt::Key_Pause,         0},                     //  19   0x13   VK_PAUSE            | Pause key
    {Qt::Key_CapsLock,      0},                     //  20   0x14   VK_CAPITAL          | Caps-Lock
    {0,                     0},                     //  21   0x15   VK_KANA / VK_HANGUL | IME Kana or Hangul mode
    {0,                     0},                     //  22   0x16   -- unassigned --
    {0,                     0},                     //  23   0x17   VK_JUNJA            | IME Junja mode
    {0,                     0},                     //  24   0x18   VK_FINAL            | IME final mode
    {0,                     0},                     //  25   0x19   VK_HANJA / VK_KANJI | IME Hanja or Kanji mode
    {0,                     0},                     //  26   0x1A   -- unassigned --
    {Qt::Key_Escape,        0},                     //  27   0x1B   VK_ESCAPE           | Esc key
    {0,                     0},                     //  28   0x1C   VK_CONVERT          | IME convert
    {0,                     0},                     //  29   0x1D   VK_NONCONVERT       | IME non-convert
    {0,                     0},                     //  30   0x1E   VK_ACCEPT           | IME accept
    {Qt::Key_Mode_switch,   0},                     //  31   0x1F   VK_MODECHANGE       | IME mode change request       | Japanese keyboards apparently?
    {Qt::Key_Space,         0},                     //  32   0x20   VK_SPACE            | Spacebar
    {Qt::Key_PageUp,        0},                     //  33   0x21   VK_PRIOR            | Page Up key
    {Qt::Key_PageDown,      0},                     //  34   0x22   VK_NEXT             | Page Down key
    {Qt::Key_End,           0},                     //  35   0x23   VK_END              | End key
    {Qt::Key_Home,          0},                     //  36   0x24   VK_HOME             | Home key
    {Qt::Key_Left,          0},                     //  37   0x25   VK_LEFT             | Left arrow key
    {Qt::Key_Up,            0},                     //  38   0x26   VK_UP               | Up arrow key
    {Qt::Key_Right,         0},                     //  39   0x27   VK_RIGHT            | Right arrow key
    {Qt::Key_Down,          0},                     //  40   0x28   VK_DOWN             | Down arrow key
    {Qt::Key_Select,        0},                     //  41   0x29   VK_SELECT           | Select key                    | Couldn't google what it is.
    {Qt::Key_Printer,       0},                     //  42   0x2A   VK_PRINT            | Print key                     | Only on nokia keyboards?
    {Qt::Key_Execute,       0},                     //  43   0x2B   VK_EXECUTE          | Execute key                   | Couldn't google what it is.
    {Qt::Key_Print,         0},                     //  44   0x2C   VK_SNAPSHOT         | Print Screen key
    {Qt::Key_Insert,        0},                     //  45   0x2D   VK_INSERT           | Ins key
    {Qt::Key_Delete,        0},                     //  46   0x2E   VK_DELETE           | Del key
    {Qt::Key_Help,          0},                     //  47   0x2F   VK_HELP             | Help key                      | On MS keyboards?
    {Qt::Key_0,             0},                     //  48   0x30   (VK_0)              | 0 key
    {Qt::Key_1,             0},                     //  49   0x31   (VK_1)              | 1 key
    {Qt::Key_2,             0},                     //  50   0x32   (VK_2)              | 2 key
    {Qt::Key_3,             0},                     //  51   0x33   (VK_3)              | 3 key
    {Qt::Key_4,             0},                     //  52   0x34   (VK_4)              | 4 key
    {Qt::Key_5,             0},                     //  53   0x35   (VK_5)              | 5 key
    {Qt::Key_6,             0},                     //  54   0x36   (VK_6)              | 6 key
    {Qt::Key_7,             0},                     //  55   0x37   (VK_7)              | 7 key
    {Qt::Key_8,             0},                     //  56   0x38   (VK_8)              | 8 key
    {Qt::Key_9,             0},                     //  57   0x39   (VK_9)              | 9 key
    {0,                     0},                     //  58   0x3A   -- unassigned --
    {0,                     0},                     //  59   0x3B   -- unassigned --
    {0,                     0},                     //  60   0x3C   -- unassigned --
    {0,                     0},                     //  61   0x3D   -- unassigned --
    {0,                     0},                     //  62   0x3E   -- unassigned --
    {0,                     0},                     //  63   0x3F   -- unassigned --
    {0,                     0},                     //  64   0x40   -- unassigned --
    {Qt::Key_A,             0},                     //  65   0x41   (VK_A)              | A key
    {Qt::Key_B,             0},                     //  66   0x42   (VK_B)              | B key
    {Qt::Key_C,             0},                     //  67   0x43   (VK_C)              | C key
    {Qt::Key_D,             0},                     //  68   0x44   (VK_D)              | D key
    {Qt::Key_E,             0},                     //  69   0x45   (VK_E)              | E key
    {Qt::Key_F,             0},                     //  70   0x46   (VK_F)              | F key
    {Qt::Key_G,             0},                     //  71   0x47   (VK_G)              | G key
    {Qt::Key_H,             0},                     //  72   0x48   (VK_H)              | H key
    {Qt::Key_I,             0},                     //  73   0x49   (VK_I)              | I key
    {Qt::Key_J,             0},                     //  74   0x4A   (VK_J)              | J key
    {Qt::Key_K,             0},                     //  75   0x4B   (VK_K)              | K key
    {Qt::Key_L,             0},                     //  76   0x4C   (VK_L)              | L key
    {Qt::Key_M,             0},                     //  77   0x4D   (VK_M)              | M key
    {Qt::Key_N,             0},                     //  78   0x4E   (VK_N)              | N key
    {Qt::Key_O,             0},                     //  79   0x4F   (VK_O)              | O key
    {Qt::Key_P,             0},                     //  80   0x50   (VK_P)              | P key
    {Qt::Key_Q,             0},                     //  81   0x51   (VK_Q)              | Q key
    {Qt::Key_R,             0},                     //  82   0x52   (VK_R)              | R key
    {Qt::Key_S,             0},                     //  83   0x53   (VK_S)              | S key
    {Qt::Key_T,             0},                     //  84   0x54   (VK_T)              | T key
    {Qt::Key_U,             0},                     //  85   0x55   (VK_U)              | U key
    {Qt::Key_V,             0},                     //  86   0x56   (VK_V)              | V key
    {Qt::Key_W,             0},                     //  87   0x57   (VK_W)              | W key
    {Qt::Key_X,             0},                     //  88   0x58   (VK_X)              | X key
    {Qt::Key_Y,             0},                     //  89   0x59   (VK_Y)              | Y key
    {Qt::Key_Z,             0},                     //  90   0x5A   (VK_Z)              | Z key
    {Qt::Key_Meta,          0},                     //  91   0x5B   VK_LWIN             | Left Windows  - MS Natural kbd
    {Qt::Key_Meta,          0},                     //  92   0x5C   VK_RWIN             | Right Windows - MS Natural kbd
    {Qt::Key_Menu,          0},                     //  93   0x5D   VK_APPS             | Application key-MS Natural kbd | Menu key, on most keyboards these days.
    {0,                     0},                     //  94   0x5E   -- reserved --
    {Qt::Key_Sleep,         0},                     //  95   0x5F   VK_SLEEP                                            | Yeah, I remember this one!
    {Qt::Key_0,             Qt::KeypadModifier},    //  96   0x60   VK_NUMPAD0          | Numeric keypad 0 key
    {Qt::Key_1,             Qt::KeypadModifier},    //  97   0x61   VK_NUMPAD1          | Numeric keypad 1 key
    {Qt::Key_2,             Qt::KeypadModifier},    //  98   0x62   VK_NUMPAD2          | Numeric keypad 2 key
    {Qt::Key_3,             Qt::KeypadModifier},    //  99   0x63   VK_NUMPAD3          | Numeric keypad 3 key
    {Qt::Key_4,             Qt::KeypadModifier},    // 100   0x64   VK_NUMPAD4          | Numeric keypad 4 key
    {Qt::Key_5,             Qt::KeypadModifier},    // 101   0x65   VK_NUMPAD5          | Numeric keypad 5 key
    {Qt::Key_6,             Qt::KeypadModifier},    // 102   0x66   VK_NUMPAD6          | Numeric keypad 6 key
    {Qt::Key_7,             Qt::KeypadModifier},    // 103   0x67   VK_NUMPAD7          | Numeric keypad 7 key
    {Qt::Key_8,             Qt::KeypadModifier},    // 104   0x68   VK_NUMPAD8          | Numeric keypad 8 key
    {Qt::Key_9,             Qt::KeypadModifier},    // 105   0x69   VK_NUMPAD9          | Numeric keypad 9 key
    {Qt::Key_Asterisk,      Qt::KeypadModifier},    // 106   0x6A   VK_MULTIPLY         | Multiply key
    {Qt::Key_Plus,          Qt::KeypadModifier},    // 107   0x6B   VK_ADD              | Add key
    {0,                     0},                     // 108   0x6C   VK_SEPARATOR        | Separator key (locale-dependent) | Apparently this just doesn't exist.
    {Qt::Key_Minus,         Qt::KeypadModifier},    // 109   0x6D   VK_SUBTRACT         | Subtract key
    {Qt::Key_Period,        Qt::KeypadModifier},    // 110   0x6E   VK_DECIMAL          | Decimal key (locale-dependent)
    {Qt::Key_Slash,         Qt::KeypadModifier},    // 111   0x6F   VK_DIVIDE           | Divide key
    {Qt::Key_F1,            0},                     // 112   0x70   VK_F1               | F1 key
    {Qt::Key_F2,            0},                     // 113   0x71   VK_F2               | F2 key
    {Qt::Key_F3,            0},                     // 114   0x72   VK_F3               | F3 key
    {Qt::Key_F4,            0},                     // 115   0x73   VK_F4               | F4 key
    {Qt::Key_F5,            0},                     // 116   0x74   VK_F5               | F5 key
    {Qt::Key_F6,            0},                     // 117   0x75   VK_F6               | F6 key
    {Qt::Key_F7,            0},                     // 118   0x76   VK_F7               | F7 key
    {Qt::Key_F8,            0},                     // 119   0x77   VK_F8               | F8 key
    {Qt::Key_F9,            0},                     // 120   0x78   VK_F9               | F9 key
    {Qt::Key_F10,           0},                     // 121   0x79   VK_F10              | F10 key
    {Qt::Key_F11,           0},                     // 122   0x7A   VK_F11              | F11 key
    {Qt::Key_F12,           0},                     // 123   0x7B   VK_F12              | F12 key
    {Qt::Key_F13,           0},                     // 124   0x7C   VK_F13              | F13 key
    {Qt::Key_F14,           0},                     // 125   0x7D   VK_F14              | F14 key
    {Qt::Key_F15,           0},                     // 126   0x7E   VK_F15              | F15 key
    {Qt::Key_F16,           0},                     // 127   0x7F   VK_F16              | F16 key
    {Qt::Key_F17,           0},                     // 128   0x80   VK_F17              | F17 key
    {Qt::Key_F18,           0},                     // 129   0x81   VK_F18              | F18 key
    {Qt::Key_F19,           0},                     // 130   0x82   VK_F19              | F19 key
    {Qt::Key_F20,           0},                     // 131   0x83   VK_F20              | F20 key
    {Qt::Key_F21,           0},                     // 132   0x84   VK_F21              | F21 key
    {Qt::Key_F22,           0},                     // 133   0x85   VK_F22              | F22 key
    {Qt::Key_F23,           0},                     // 134   0x86   VK_F23              | F23 key
    {Qt::Key_F24,           0},                     // 135   0x87   VK_F24              | F24 key
    {0,                     0},                     // 136   0x88   -- unassigned --
    {0,                     0},                     // 137   0x89   -- unassigned --
    {0,                     0},                     // 138   0x8A   -- unassigned --
    {0,                     0},                     // 139   0x8B   -- unassigned --
    {0,                     0},                     // 140   0x8C   -- unassigned --
    {0,                     0},                     // 141   0x8D   -- unassigned --
    {0,                     0},                     // 142   0x8E   -- unassigned --
    {0,                     0},                     // 143   0x8F   -- unassigned --
    {Qt::Key_NumLock,       Qt::KeypadModifier},    // 144   0x90   VK_NUMLOCK          | Num Lock key
    {Qt::Key_ScrollLock,    0},                     // 145   0x91   VK_SCROLL           | Scroll Lock key
                                                    // Fujitsu/OASYS kbd --------------------
    {-1,                    0},                     // 146   0x92   VK_OEM_FJ_JISHO     | 'Dictionary' key /
                                                    //              VK_OEM_NEC_EQUAL  = key on numpad on NEC PC-9800 kbd
    {Qt::Key_Massyo,        0},                     // 147   0x93   VK_OEM_FJ_MASSHOU   | 'Unregister word' key
    {Qt::Key_Touroku,       0},                     // 148   0x94   VK_OEM_FJ_TOUROKU   | 'Register word' key
    {-1,                    0},                     // 149   0x95   VK_OEM_FJ_LOYA  | 'Left OYAYUBI' key
    {-1,                    0},                     // 150   0x96   VK_OEM_FJ_ROYA  | 'Right OYAYUBI' key
    {0,                     0},                     // 151   0x97   -- unassigned --
    {0,                     0},                     // 152   0x98   -- unassigned --
    {0,                     0},                     // 153   0x99   -- unassigned --
    {0,                     0},                     // 154   0x9A   -- unassigned --
    {0,                     0},                     // 155   0x9B   -- unassigned --
    {0,                     0},                     // 156   0x9C   -- unassigned --
    {0,                     0},                     // 157   0x9D   -- unassigned --
    {0,                     0},                     // 158   0x9E   -- unassigned --
    {0,                     0},                     // 159   0x9F   -- unassigned --
    {Qt::Key_Shift,         0},                     // 160   0xA0   VK_LSHIFT           | Left Shift key
    {Qt::Key_Shift,         0},                     // 161   0xA1   VK_RSHIFT           | Right Shift key
    {Qt::Key_Control,       0},                     // 162   0xA2   VK_LCONTROL         | Left Ctrl key
    {Qt::Key_Control,       0},                     // 163   0xA3   VK_RCONTROL         | Right Ctrl key
    {Qt::Key_Alt,           0},                     // 164   0xA4   VK_LMENU            | Left Menu key
    {Qt::Key_Alt,           0},                     // 165   0xA5   VK_RMENU            | Right Menu key
    {Qt::Key_Back,          0},                     // 166   0xA6   VK_BROWSER_BACK     | Browser Back key              | MS Natural KBD ? {
    {Qt::Key_Forward,       0},                     // 167   0xA7   VK_BROWSER_FORWARD  | Browser Forward key           |
    {Qt::Key_Refresh,       0},                     // 168   0xA8   VK_BROWSER_REFRESH  | Browser Refresh key           |
    {Qt::Key_Stop,          0},                     // 169   0xA9   VK_BROWSER_STOP     | Browser Stop key              |
    {Qt::Key_Search,        0},                     // 170   0xAA   VK_BROWSER_SEARCH   | Browser Search key            |
    {Qt::Key_Favorites,     0},                     // 171   0xAB   VK_BROWSER_FAVORITES| Browser Favorites key         |
    {Qt::Key_HomePage,      0},                     // 172   0xAC   VK_BROWSER_HOME     | Browser Start and Home key    |
    {Qt::Key_VolumeMute,    0},                     // 173   0xAD   VK_VOLUME_MUTE      | Volume Mute key               |
    {Qt::Key_VolumeDown,    0},                     // 174   0xAE   VK_VOLUME_DOWN      | Volume Down key               |
    {Qt::Key_VolumeUp,      0},                     // 175   0xAF   VK_VOLUME_UP        | Volume Up key                 |
    {Qt::Key_MediaNext,     0},                     // 176   0xB0   VK_MEDIA_NEXT_TRACK | Next Track key                |
    {Qt::Key_MediaPrevious, 0},                     // 177 0xB1   VK_MEDIA_PREV_TRACK | Previous Track key              |
    {Qt::Key_MediaStop,     0},                     // 178   0xB2   VK_MEDIA_STOP       | Stop Media key                |
    {Qt::Key_MediaPlay,     0},                     // 179   0xB3   VK_MEDIA_PLAY_PAUSE | Play/Pause Media key          |
    {Qt::Key_LaunchMail,    0},                     // 180   0xB4   VK_LAUNCH_MAIL      | Start Mail key                |
    {Qt::Key_LaunchMedia,   0},                     // 181   0xB5   VK_LAUNCH_MEDIA_SELECT Select Media key             |
    {Qt::Key_Launch0,       0},                     // 182   0xB6   VK_LAUNCH_APP1      | Start Application 1 key       |
    {Qt::Key_Launch1,       0},                     // 183   0xB7   VK_LAUNCH_APP2      | Start Application 2 key       | }
    {0,                     0},                     // 184   0xB8   -- reserved --
    {0,                     0},                     // 185   0xB9   -- reserved --
    {Qt::Key_Semicolon,     0},                     // 186   0xBA   VK_OEM_1            | ';:' for US                   | ';' next to return
    {Qt::Key_Equal,         0},                     // 187   0xBB   VK_OEM_PLUS         | '+' any country               | '=' next to backslash
    {Qt::Key_Comma,         0},                     // 188   0xBC   VK_OEM_COMMA        | ',' any country               | ',' next to rshift
    {Qt::Key_Minus,         0},                     // 189   0xBD   VK_OEM_MINUS        | '-' any country               | '-' next to backslash
    {Qt::Key_Period,        0},                     // 190   0xBE   VK_OEM_PERIOD       | '.' any country               | '.' next to rshift
    {Qt::Key_Slash,         0},                     // 191   0xBF   VK_OEM_2            | '/?' for US                   | '/' next to shift
    {Qt::Key_QuoteLeft,     0},                     // 192   0xC0   VK_OEM_3            | '`~' for US                   | '`' aka console
    {0,                     0},                     // 193   0xC1   -- reserved --
    {0,                     0},                     // 194   0xC2   -- reserved --
    {0,                     0},                     // 195   0xC3   -- reserved --
    {0,                     0},                     // 196   0xC4   -- reserved --
    {0,                     0},                     // 197   0xC5   -- reserved --
    {0,                     0},                     // 198   0xC6   -- reserved --
    {0,                     0},                     // 199   0xC7   -- reserved --
    {0,                     0},                     // 200   0xC8   -- reserved --
    {0,                     0},                     // 201   0xC9   -- reserved --
    {0,                     0},                     // 202   0xCA   -- reserved --
    {0,                     0},                     // 203   0xCB   -- reserved --
    {0,                     0},                     // 204   0xCC   -- reserved --
    {0,                     0},                     // 205   0xCD   -- reserved --
    {0,                     0},                     // 206   0xCE   -- reserved --
    {0,                     0},                     // 207   0xCF   -- reserved --
    {0,                     0},                     // 208   0xD0   -- reserved --
    {0,                     0},                     // 209   0xD1   -- reserved --
    {0,                     0},                     // 210   0xD2   -- reserved --
    {0,                     0},                     // 211   0xD3   -- reserved --
    {0,                     0},                     // 212   0xD4   -- reserved --
    {0,                     0},                     // 213   0xD5   -- reserved --
    {0,                     0},                     // 214   0xD6   -- reserved --
    {0,                     0},                     // 215   0xD7   -- reserved --
    {0,                     0},                     // 216   0xD8   -- unassigned --
    {0,                     0},                     // 217   0xD9   -- unassigned --
    {0,                     0},                     // 218   0xDA   -- unassigned --
    {Qt::Key_BracketLeft,   0},                     // 219   0xDB   VK_OEM_4            | '[{' for US                   | '[' next to return
    {Qt::Key_Backslash,     0},                     // 220   0xDC   VK_OEM_5            | '\|' for US                   | '\' next to return
    {Qt::Key_BracketRight,  0},                     // 221   0xDD   VK_OEM_6            | ']}' for US                   | ']' next to return
    {Qt::Key_Apostrophe,    0},                     // 222   0xDE   VK_OEM_7            | ''"' for US                   | ''' next to return
    {-1,                    0},                     // 223   0xDF   VK_OEM_8                                            | French '!', TODO
    {0,                     0},                     // 224   0xE0   -- reserved --
    {0,                     0},                     // 225   0xE1   VK_OEM_AX           | 'AX' key on Japanese AX kbd
    {0,                     0},                     // 226   0xE2   VK_OEM_102          | "<>" or "\|" on RT 102-key kbd
    {0,                     0},                     // 227   0xE3   VK_ICO_HELP         | Help key on ICO
    {0,                     0},                     // 228   0xE4   VK_ICO_00           | 00 key on ICO
    {0,                     0},                     // 229   0xE5   VK_PROCESSKEY       | IME Process key
    {0,                     0},                     // 230   0xE6   VK_ICO_CLEAR        |
    {0,                     0},                     // 231   0xE7   VK_PACKET           | Unicode char as keystrokes
    {0,                     0},                     // 232   0xE8   -- unassigned --
                                                    // Nokia/Ericsson definitions ---------------
    {0,                     0},                     // 233   0xE9   VK_OEM_RESET
    {0,                     0},                     // 234   0xEA   VK_OEM_JUMP
    {0,                     0},                     // 235   0xEB   VK_OEM_PA1
    {0,                     0},                     // 236   0xEC   VK_OEM_PA2
    {0,                     0},                     // 237   0xED   VK_OEM_PA3
    {0,                     0},                     // 238   0xEE   VK_OEM_WSCTRL
    {0,                     0},                     // 239   0xEF   VK_OEM_CUSEL
    {0,                     0},                     // 240   0xF0   VK_OEM_ATTN
    {0,                     0},                     // 241   0xF1   VK_OEM_FINISH
    {0,                     0},                     // 242   0xF2   VK_OEM_COPY
    {0,                     0},                     // 243   0xF3   VK_OEM_AUTO
    {0,                     0},                     // 244   0xF4   VK_OEM_ENLW
    {0,                     0},                     // 245   0xF5   VK_OEM_BACKTAB
    {0,                     0},                     // 246   0xF6   VK_ATTN             | Attn key
    {0,                     0},                     // 247   0xF7   VK_CRSEL            | CrSel key
    {0,                     0},                     // 248   0xF8   VK_EXSEL            | ExSel key
    {0,                     0},                     // 249   0xF9   VK_EREOF            | Erase EOF key
    {Qt::Key_Play,          0},                     // 250   0xFA   VK_PLAY             | Play key
    {Qt::Key_Zoom,          0},                     // 251   0xFB   VK_ZOOM             | Zoom key
    {0,                     0},                     // 252   0xFC   VK_NONAME           | Reserved
    {0,                     0},                     // 253   0xFD   VK_PA1              | PA1 key
    {-1,                    0},                     // 254   0xFE   VK_OEM_CLEAR        | Clear key
    {-1,                    0}
}};

WinShortcutManager::WinShortcutManager(WinSharedEventWindow* eventWindow) :
    m_eventWindow(eventWindow)
{
    for (quint32 winKey = 0; winKey < KeyMapping.size(); winKey++) {
        int qtKey = KeyMapping[winKey].key;
        switch (qtKey) {
        case -1:
        case 0:
        case Qt::Key_Meta:
        case Qt::Key_Shift:
        case Qt::Key_Alt:
        case Qt::Key_Control:
            continue;
        }

        QKeyCombination combinedKey = static_cast<Qt::Key>(qtKey) | KeyMapping[winKey].mods;

        assert(!m_winKeyByQtKey.contains(combinedKey.toCombined()));
        m_winKeyByQtKey[combinedKey.toCombined()] = winKey;

        m_bindableKeys.insert(combinedKey);
    }

    connect(m_eventWindow, &WinSharedEventWindow::hotkey, this, &WinShortcutManager::dispatchEvent);
}

WinShortcutManager::~WinShortcutManager() {}

std::unordered_set<QKeyCombination> WinShortcutManager::bindableKeys() const {
    return m_bindableKeys;
}

PlatformShortcutNotifier* WinShortcutManager::createShortcutNotifier(const QKeyCombination& shortcut) const {
    Qt::Key key = shortcut.key();
    Qt::KeyboardModifiers mods = shortcut.keyboardModifiers();
    quint32 nativeKey = 0;
    quint32 nativeMods = 0;
    convertToNativeKey(key, mods, &nativeKey, &nativeMods);
    if (nativeKey == 0) {
        qWarning() << "Could not convert" << shortcut << "to native key combination.";
        return nullptr;
    }

    if (!apicall(RegisterHotKey(reinterpret_cast<HWND>(m_eventWindow->winId()), m_nextId, nativeMods, nativeKey)))
        return nullptr;

    WinShortcutNotifier* notifier = new WinShortcutNotifier(m_nextId);
    connect(notifier, &WinShortcutNotifier::aboutToBeDestroyed, this, &WinShortcutManager::removeShortcutNotifier);
    m_notifierById[m_nextId] = notifier;
    m_nextId++;
    return notifier;
}

void WinShortcutManager::dispatchEvent(MSG* message) {
    assert(message->message == WM_HOTKEY);

    WinShortcutNotifier* notifier = value_or(m_notifierById, message->wParam);
    if (!notifier) {
        qWarning() << "Received hotkey message w/o notifier, lParam = " << message->lParam << ", wParam = " << message->wParam;
        return;
    }

    emit notifier->activated();
}

void WinShortcutManager::removeShortcutNotifier(int id) {
    apicall(UnregisterHotKey(reinterpret_cast<HWND>(m_eventWindow->winId()), id));

    m_notifierById.erase(id);
}

void WinShortcutManager::convertToNativeKey(Qt::Key key, Qt::KeyboardModifiers mods, quint32* outKey, quint32* outMods) const {
    int winMods = 0;
    if (mods & Qt::ShiftModifier)
        winMods |= MOD_SHIFT;
    if (mods & Qt::ControlModifier)
        winMods |= MOD_CONTROL;
    if (mods & Qt::AltModifier)
        winMods |= MOD_ALT;
    if (mods & Qt::MetaModifier)
        winMods |= MOD_WIN;

    int qtKey = key;
    if (mods & Qt::KeypadModifier)
        qtKey |= Qt::KeypadModifier;

    *outKey = value_or(m_winKeyByQtKey, qtKey, 0);
    *outMods = winMods;
}

