#include "keyboard_mouse_control.h"
#include "support_func.h"
#include <windows.h>
#include <mmsystem.h>

std::atomic<bool> g_active(false);
std::atomic<bool> g_running(true);

void KeyboardHook() {
    bool f6Pressed = false;

    while (g_running) {
        // Проверяем нажатие F6
        if (GetAsyncKeyState(VK_F6) & 0x8000) {
            if (!f6Pressed) {
                f6Pressed = true;
                g_active = !g_active; // Переключаем состояние

                if (g_active) {
                    PlaySound(TEXT("MailBeep"), NULL, SND_ALIAS | SND_ASYNC);
                } else {
                    PlaySound(TEXT("SystemStart"), NULL, SND_ALIAS | SND_ASYNC);
                }
            }
        } else {
            f6Pressed = false;
        }

        Sleep(50); // Проверяем нажатия каждые 50мс
    }
}

void first_enters(){

    Sleep(650);

    sendKeyPress(VK_RETURN);

    Sleep(195);

    sendKeyPress(VK_RETURN);

    Sleep(800);
}

void first_slots(const int& delay){

    sendKeyPress('Y');

    Sleep(195);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_RETURN);

    Sleep(195);

    sendKeyPress(VK_RETURN);

    Sleep(4000);

    sendKeyPress(VK_RETURN);

    Sleep(250);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);
}

void second_slots(const int& delay){

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress('Y');

    Sleep(195);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_RETURN);

    Sleep(195);

    sendKeyPress(VK_RETURN);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(4000);

    sendKeyPress(VK_RETURN);

    Sleep(250);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);
}

void third_slots(const int& delay){
    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress('Y');

    Sleep(195);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_RETURN);

    Sleep(195);

    sendKeyPress(VK_RETURN);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(4000);

    sendKeyPress(VK_RETURN);

    Sleep(250);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);
}

void fourth_slots(const int& delay){

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress('Y');

    Sleep(195);

    sendKeyPress(VK_DOWN);

    Sleep(delay);

    sendKeyPress(VK_RETURN);

    Sleep(195);

    sendKeyPress(VK_RETURN);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(4000);

    sendKeyPress(VK_RETURN);

    Sleep(250);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);

    sendKeyPress(VK_ESCAPE);

    Sleep(delay);

}

