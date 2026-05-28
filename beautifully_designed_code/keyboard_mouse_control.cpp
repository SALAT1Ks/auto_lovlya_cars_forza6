#include "keyboard_mouse_control.h"

void moveMouseAbsolute(int x, int y) {
    // Преобразование в координаты экрана (0-65535)
    int screenX = (x * 65535) / GetSystemMetrics(SM_CXSCREEN);
    int screenY = (y * 65535) / GetSystemMetrics(SM_CYSCREEN);

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = screenX;
    input.mi.dy = screenY;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

void sendKeyPress(WORD vkCode) {
    INPUT input = {0};

    // Нажатие клавиши
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    SendInput(1, &input, sizeof(INPUT));

    // Отпускание клавиши
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}
