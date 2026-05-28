#ifndef KEYBOARD_MOUSE_CONTROL_H
#define KEYBOARD_MOUSE_CONTROL_H

#include <windows.h>
#include <mmsystem.h>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

#pragma comment(lib, "winmm.lib")

using namespace  std;

void moveMouseAbsolute(int x, int y);

void sendKeyPress(WORD vkCode);

#endif // KEYBOARD_MOUSE_CONTROL_H
