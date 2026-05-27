#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")
using namespace  std;

std::atomic<bool> g_active(false);
std::atomic<bool> g_running(true);


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


    cout <<":" << vkCode << ":" << endl;
    // Нажатие клавиши
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    SendInput(1, &input, sizeof(INPUT));

    // Отпускание клавиши
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}


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

struct MonitorInfo {
    int left;
    int top;
    int width;
    int height;
    bool isPrimary;
};

vector<MonitorInfo> GetMonitorsInfo() {
    vector<MonitorInfo> monitors;

    EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
        auto& monitors = *reinterpret_cast<vector<MonitorInfo>*>(dwData);

        MONITORINFOEX monitorInfo;
        monitorInfo.cbSize = sizeof(monitorInfo);

        if (GetMonitorInfo(hMonitor, &monitorInfo)) {
            MonitorInfo info;
            info.left = monitorInfo.rcMonitor.left;
            info.top = monitorInfo.rcMonitor.top;
            info.width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
            info.height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
            info.isPrimary = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0;
            monitors.push_back(info);
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(&monitors));

    return monitors;
}


void moveMouseOnMonitor(int monitorIndex, int xRelative, int yRelative) {
    auto monitors = GetMonitorsInfo();

    if (monitors.empty()) {
        moveMouseAbsolute(xRelative, yRelative);
        return;
    }

    // Проверяем, существует ли запрошенный монитор
    if (monitorIndex >= monitors.size()) {
        cout << "Монитор с индексом " << monitorIndex << " не найден. Используется монитор 0." << endl;
        monitorIndex = 0;
    }

    MonitorInfo& monitor = monitors[monitorIndex];

    // Преобразуем относительные координаты в абсолютные
    int x = monitor.left + min(xRelative, monitor.width - 1);
    int y = monitor.top + min(yRelative, monitor.height - 1);

    moveMouseAbsolute(x, y);
}

COLORREF getPixelColorOnMonitor(int monitorIndex, int xRelative, int yRelative) {
    auto monitors = GetMonitorsInfo();
    if (monitors.empty() || monitorIndex >= monitors.size()) {
        monitorIndex = 0;
    }

    MonitorInfo& monitor = monitors[monitorIndex];
    int absoluteX = monitor.left + min(xRelative, monitor.width - 1);
    int absoluteY = monitor.top + min(yRelative, monitor.height - 1);

    HDC hdc = GetDC(NULL);
    COLORREF color = GetPixel(hdc, absoluteX, absoluteY);
    ReleaseDC(NULL, hdc);

    return color;
}


bool isCarSoldOut(int monitorIndex, int startX, int startY) {
    HDC hdc = GetDC(NULL);
    // Получаем реальные координаты с учетом монитора
    auto monitors = GetMonitorsInfo();
    MonitorInfo& monitor = monitors[monitorIndex];
    int absX = monitor.left + startX;
    int absY = monitor.top + startY;

    // Сканируем квадрат 24 на 24 пикселей с шагом в 4 пикселя (для скорости)
    for (int x = 0; x < 24; x += 4) {
        for (int y = 0; y < 24; y += 4) {
            COLORREF color = GetPixel(hdc, absX + x, absY + y);
            int r = GetRValue(color);
            int g = GetGValue(color);
            int b = GetBValue(color);

            // Ищем салатово-желтую плашку "ПРОДАНО!"
            // Она очень яркая (R и G высокие, B — низкий)
            if (r == 234 && g == 222 && b == 0) {
                ReleaseDC(NULL, hdc);

                cout << "true";
                return true; // Нашли плашку! Машина выкуплена.
            }
        }
    }
    cout << "false";
    ReleaseDC(NULL, hdc);
    return false; // Плашки нет, машина свободна!
}

bool Backgroud_is_White(int monitorIndex, int startX, int startY) {
    HDC hdc = GetDC(NULL);
    // Получаем реальные координаты с учетом монитора
    auto monitors = GetMonitorsInfo();
    MonitorInfo& monitor = monitors[monitorIndex];
    int absX = monitor.left + startX;
    int absY = monitor.top + startY;

    // Сканируем квадрат 15 на 15 пикселей с шагом в 3 пикселя (для скорости)
    for (int x = 0; x < 15; x += 3) {
        for (int y = 0; y < 15; y += 3) {
            COLORREF color = GetPixel(hdc, absX + x, absY + y);
            int r = GetRValue(color);
            int g = GetGValue(color);
            int b = GetBValue(color);

            // Ищем салатово-желтую плашку "ПРОДАНО!"
            // Она очень яркая (R и G высокие, B — низкий)
            if (r > 250 && g > 250 && b > 250) {
                ReleaseDC(NULL, hdc);

                cout << "true";
                return true; // Нашли плашку! Машина выкуплена.
            }
        }
    }
    cout << "false";
    ReleaseDC(NULL, hdc);
    return false; // Плашки нет, машина свободна!
}

int main(){

    system("chcp 65001>nul");

    std::thread keyboard(KeyboardHook);

    // Получаем информацию о мониторах
    auto monitors = GetMonitorsInfo();
    cout << "Найдено мониторов: " << monitors.size() << endl;
    for (size_t i = 0; i < monitors.size(); i++) {
        cout << "Монитор " << i+1 << ": "
             << monitors[i].width << "x" << monitors[i].height
             << " (X:" << monitors[i].left << ", Y:" << monitors[i].top << ")"
             << (monitors[i].isPrimary ? " [Основной]" : "") << endl;
    }
    int targetMonitor = 0;
    cout << "Введите номер монитора: ";
    cin >> targetMonitor;
    cout << endl;
    --targetMonitor;

    if (targetMonitor >= monitors.size()) {
        cout << "Второй монитор не найден! Работаем на основном мониторе." << endl;
        targetMonitor = 0;
    } else {
        cout << "Будем работать на мониторе " << targetMonitor+1 << endl;
    }


    cout << "задержка в МС: ";

    int delay = 0;

    cin >> delay;
    cout << "Скрипт готов к работе. Нажмите F6 для старта/паузы." << endl;

    // bool control_round_one = isCarSoldOut(targetMonitor, 190, 235);
    // bool control_round_two = isCarSoldOut(targetMonitor, 190, 440);
    // bool control_round_three = isCarSoldOut(targetMonitor, 190, 640);
    // bool control_round_four = isCarSoldOut(targetMonitor, 190, 840);

    bool control_white_round = Backgroud_is_White(targetMonitor, 777, 228);


    COLORREF control_pixel_one = getPixelColorOnMonitor(targetMonitor, 190, 235);
    COLORREF control_pixel_two = getPixelColorOnMonitor(targetMonitor, 190, 440);
    COLORREF control_pixel_three = getPixelColorOnMonitor(targetMonitor, 190, 640);
    COLORREF control_pixel_four = getPixelColorOnMonitor(targetMonitor, 190, 840);

    while(g_running){
        while(g_active){

            Sleep(650);

            cout << 'a' << endl;

            sendKeyPress(VK_RETURN);

            Sleep(195);

            sendKeyPress(VK_RETURN);

            Sleep(800);

            control_white_round = Backgroud_is_White(targetMonitor, 777, 228);

            // control_round_one = isCarSoldOut(targetMonitor, 190, 235);
            // control_round_two = isCarSoldOut(targetMonitor, 190, 440);
            // control_round_three = isCarSoldOut(targetMonitor, 190, 640);
            // control_round_four = isCarSoldOut(targetMonitor, 190, 840);

            // control_pixel_one = getPixelColorOnMonitor(targetMonitor, 194, 235);
            // control_pixel_two = getPixelColorOnMonitor(targetMonitor, 194, 440);
            // control_pixel_three = getPixelColorOnMonitor(targetMonitor, 194, 640);
            // control_pixel_four = getPixelColorOnMonitor(targetMonitor, 194, 840);

            if (!control_white_round){

                cout << 1 << endl;

                sendKeyPress(VK_ESCAPE);

            }

            else{

                while (true){
                    if (getPixelColorOnMonitor(targetMonitor, 777, 228) == RGB(255, 255, 255)){
                        break;
                    }
                }


                if (getPixelColorOnMonitor(targetMonitor, 194, 235) != RGB(234, 222, 0)){

                    cout << 2;

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

                else if (getPixelColorOnMonitor(targetMonitor, 194, 440) != RGB(234, 222, 0) and getPixelColorOnMonitor(targetMonitor, 194, 440) != RGB(29, 29, 29)){

                    cout << 3;

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

                else if (getPixelColorOnMonitor(targetMonitor, 194, 640) != RGB(234, 222, 0) and getPixelColorOnMonitor(targetMonitor, 194, 640) != RGB(29, 29, 29)){

                    cout << 4;

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

                else if (getPixelColorOnMonitor(targetMonitor, 194, 840) != RGB(234, 222, 0) and getPixelColorOnMonitor(targetMonitor, 194, 840) != RGB(29, 29, 29)){

                    cout << 5;

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


            }

            cout << "нет машин в первых 4-ёх слотах" << endl;

            sendKeyPress(VK_ESCAPE);



        }
    }

    return 0;

}
