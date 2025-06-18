#ifndef TDD_ON_ESCAPE_H
#define TDD_ON_ESCAPE_H

#include <stdio.h>


#if defined(__WATCOMC__)
    #if defined(_BIOS_H)
        #include <bios.h>
        // Checks for ESC in non-blocking way (BIOS scan code 0x011B) and performs action eg ON_ESCAPE(break;)
        #define ON_ESCAPE(action)  \
            if (_bios_keybrd(_KEYBRD_READY)) { \
                if (_bios_keybrd(_KEYBRD_READ) == 0x011B) { \
                    action \
                } \
            }
        #define PRESS_ENTER \
            do { \
                printf("\nPress Enter to continue..."); \
                while (_bios_keybrd(_KEYBRD_READ) != 0x1C0D); /* Enter scan code */ \
            } while(0)
    #else
        #include <conio.h> // Watcom's conio.h (kbhit/getch)
        #define ON_ESCAPE(action) \
            if (kbhit()) { \
                if (getch() == 27) { \
                    action \
                } \
            }
        #define PRESS_ENTER \
            do { \
                printf("\nPress Enter to continue..."); \
                fflush(stdout); \
                while (getchar() != '\n'); /* Wait for Enter */ \
            } while(0)
    #endif
#else
    // Define stubs for Clang
    #define ON_ESCAPE(action) (0)
    #define PRESS_ENTER
#endif

#endif
