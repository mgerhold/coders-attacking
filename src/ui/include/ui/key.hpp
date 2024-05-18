#pragma once

namespace ui {
    enum class Key {
        Null = 0, // Key: NULL, used for no key pressed
        // Alphanumeric keys
        Apostrophe = 39,   // Key: '
        Comma = 44,        // Key: ,
        Minus = 45,        // Key: -
        Period = 46,       // Key: .
        Slash = 47,        // Key: /
        Zero = 48,         // Key: 0
        One = 49,          // Key: 1
        Two = 50,          // Key: 2
        Three = 51,        // Key: 3
        Four = 52,         // Key: 4
        Five = 53,         // Key: 5
        Six = 54,          // Key: 6
        Seven = 55,        // Key: 7
        Eight = 56,        // Key: 8
        Nine = 57,         // Key: 9
        Semicolon = 59,    // Key: ;
        Equal = 61,        // Key: =
        A = 65,            // Key: A | a
        B = 66,            // Key: B | b
        C = 67,            // Key: C | c
        D = 68,            // Key: D | d
        E = 69,            // Key: E | e
        F = 70,            // Key: F | f
        G = 71,            // Key: G | g
        H = 72,            // Key: H | h
        I = 73,            // Key: I | i
        J = 74,            // Key: J | j
        K = 75,            // Key: K | k
        L = 76,            // Key: L | l
        M = 77,            // Key: M | m
        N = 78,            // Key: N | n
        O = 79,            // Key: O | o
        P = 80,            // Key: P | p
        Q = 81,            // Key: Q | q
        R = 82,            // Key: R | r
        S = 83,            // Key: S | s
        T = 84,            // Key: T | t
        U = 85,            // Key: U | u
        V = 86,            // Key: V | v
        W = 87,            // Key: W | w
        X = 88,            // Key: X | x
        Y = 89,            // Key: Y | y
        Z = 90,            // Key: Z | z
        LeftBracket = 91,  // Key: [
        Backslash = 92,    // Key: '\'
        RightBracket = 93, // Key: ]
        Grave = 96,        // Key: `
        // Function keys
        Space = 32,         // Key: Space
        Escape = 256,       // Key: Esc
        Enter = 257,        // Key: Enter
        Tab = 258,          // Key: Tab
        Backspace = 259,    // Key: Backspace
        Insert = 260,       // Key: Ins
        Delete = 261,       // Key: Del
        Right = 262,        // Key: Cursor right
        Left = 263,         // Key: Cursor left
        Down = 264,         // Key: Cursor down
        Up = 265,           // Key: Cursor up
        PageUp = 266,       // Key: Page up
        PageDown = 267,     // Key: Page down
        Home = 268,         // Key: Home
        End = 269,          // Key: End
        CapsLock = 280,     // Key: Caps lock
        ScrollLock = 281,   // Key: Scroll down
        NumLock = 282,      // Key: Num lock
        PrintScreen = 283,  // Key: Print screen
        Pause = 284,        // Key: Pause
        F1 = 290,           // Key: F1
        F2 = 291,           // Key: F2
        F3 = 292,           // Key: F3
        F4 = 293,           // Key: F4
        F5 = 294,           // Key: F5
        F6 = 295,           // Key: F6
        F7 = 296,           // Key: F7
        F8 = 297,           // Key: F8
        F9 = 298,           // Key: F9
        F10 = 299,          // Key: F10
        F11 = 300,          // Key: F11
        F12 = 301,          // Key: F12
        LeftShift = 340,    // Key: Shift left
        LeftControl = 341,  // Key: Control left
        LeftAlt = 342,      // Key: Alt left
        LeftSuper = 343,    // Key: Super left
        RightShift = 344,   // Key: Shift right
        RightControl = 345, // Key: Control right
        RightAlt = 346,     // Key: Alt right
        RightSuper = 347,   // Key: Super right
        KbMenu = 348,       // Key: KB menu
        // Keypad keys
        Kp0 = 320,        // Key: Keypad 0
        Kp1 = 321,        // Key: Keypad 1
        Kp2 = 322,        // Key: Keypad 2
        Kp3 = 323,        // Key: Keypad 3
        Kp4 = 324,        // Key: Keypad 4
        Kp5 = 325,        // Key: Keypad 5
        Kp6 = 326,        // Key: Keypad 6
        Kp7 = 327,        // Key: Keypad 7
        Kp8 = 328,        // Key: Keypad 8
        Kp9 = 329,        // Key: Keypad 9
        KpDecimal = 330,  // Key: Keypad .
        KpDivide = 331,   // Key: Keypad /
        KpMultiply = 332, // Key: Keypad *
        KpSubtract = 333, // Key: Keypad -
        KpAdd = 334,      // Key: Keypad +
        KpEnter = 335,    // Key: Keypad Enter
        KpEqual = 336,    // Key: Keypad =
        // Android key buttons
        Back = 4,       // Key: Android back button
        Menu = 82,      // Key: Android menu button
        VolumeUp = 24,  // Key: Android volume up button
        VolumeDown = 25 // Key: Android volume down button
    };

    constexpr char const* to_string(Key const key) {
        switch (key) {
            case Key::Null:
                return "Null";
            case Key::Apostrophe:
                return "Apostrophe";
            case Key::Comma:
                return "Comma";
            case Key::Minus:
                return "Minus";
            case Key::Period:
                return "Period";
            case Key::Slash:
                return "Slash";
            case Key::Zero:
                return "Zero";
            case Key::One:
                return "One";
            case Key::Two:
                return "Two";
            case Key::Three:
                return "Three";
            case Key::Four:
                return "Four";
            case Key::Five:
                return "Five";
            case Key::Six:
                return "Six";
            case Key::Seven:
                return "Seven";
            case Key::Eight:
                return "Eight";
            case Key::Nine:
                return "Nine";
            case Key::Semicolon:
                return "Semicolon";
            case Key::Equal:
                return "Equal";
            case Key::A:
                return "A";
            case Key::B:
                return "B";
            case Key::C:
                return "C";
            case Key::D:
                return "D";
            case Key::E:
                return "E";
            case Key::F:
                return "F";
            case Key::G:
                return "G";
            case Key::H:
                return "H";
            case Key::I:
                return "I";
            case Key::J:
                return "J";
            case Key::K:
                return "K";
            case Key::L:
                return "L";
            case Key::M:
                return "M";
            case Key::N:
                return "N";
            case Key::O:
                return "O";
            case Key::P:
                return "P";
            case Key::Q:
                return "Q";
            case Key::R:
                return "R";
            case Key::S:
                return "S";
            case Key::T:
                return "T";
            case Key::U:
                return "U";
            case Key::V:
                return "V";
            case Key::W:
                return "W";
            case Key::X:
                return "X";
            case Key::Y:
                return "Y";
            case Key::Z:
                return "Z";
            case Key::LeftBracket:
                return "LeftBracket";
            case Key::Backslash:
                return "Backslash";
            case Key::RightBracket:
                return "RightBracket";
            case Key::Grave:
                return "Grave";
            case Key::Space:
                return "Space";
            case Key::Escape:
                return "Escape";
            case Key::Enter:
                return "Enter";
            case Key::Tab:
                return "Tab";
            case Key::Backspace:
                return "Backspace";
            case Key::Insert:
                return "Insert";
            case Key::Delete:
                return "Delete";
            case Key::Right:
                return "Right";
            case Key::Left:
                return "Left";
            case Key::Down:
                return "Down";
            case Key::Up:
                return "Up";
            case Key::PageUp:
                return "PageUp";
            case Key::PageDown:
                return "PageDown";
            case Key::Home:
                return "Home";
            case Key::End:
                return "End";
            case Key::CapsLock:
                return "CapsLock";
            case Key::ScrollLock:
                return "ScrollLock";
            case Key::NumLock:
                return "NumLock";
            case Key::PrintScreen:
                return "PrintScreen";
            case Key::Pause:
                return "Pause";
            case Key::F1:
                return "F1";
            case Key::F2:
                return "F2";
            case Key::F3:
                return "F3";
            case Key::F4:
                return "F4";
            case Key::F5:
                return "F5";
            case Key::F6:
                return "F6";
            case Key::F7:
                return "F7";
            case Key::F8:
                return "F8";
            case Key::F9:
                return "F9";
            case Key::F10:
                return "F10";
            case Key::F11:
                return "F11";
            case Key::F12:
                return "F12";
            case Key::LeftShift:
                return "LeftShift";
            case Key::LeftControl:
                return "LeftControl";
            case Key::LeftAlt:
                return "LeftAlt";
            case Key::LeftSuper:
                return "LeftSuper";
            case Key::RightShift:
                return "RightShift";
            case Key::RightControl:
                return "RightControl";
            case Key::RightAlt:
                return "RightAlt";
            case Key::RightSuper:
                return "RightSuper";
            case Key::KbMenu:
                return "KbMenu";
            case Key::Kp0:
                return "Kp0";
            case Key::Kp1:
                return "Kp1";
            case Key::Kp2:
                return "Kp2";
            case Key::Kp3:
                return "Kp3";
            case Key::Kp4:
                return "Kp4";
            case Key::Kp5:
                return "Kp5";
            case Key::Kp6:
                return "Kp6";
            case Key::Kp7:
                return "Kp7";
            case Key::Kp8:
                return "Kp8";
            case Key::Kp9:
                return "Kp9";
            case Key::KpDecimal:
                return "KpDecimal";
            case Key::KpDivide:
                return "KpDivide";
            case Key::KpMultiply:
                return "KpMultiply";
            case Key::KpSubtract:
                return "KpSubtract";
            case Key::KpAdd:
                return "KpAdd";
            case Key::KpEnter:
                return "KpEnter";
            case Key::KpEqual:
                return "KpEqual";
            case Key::Back:
                return "Back";
            case Key::VolumeUp:
                return "VolumeUp";
            case Key::VolumeDown:
                return "VolumeDown";
            default:
                return "UnknownKey";
        }
    }
} // namespace ui
