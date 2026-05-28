#include "monitor_source.h"

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

// Функция проверки пикселя по процентам
COLORREF getPixelColorOnMonitorPercent(int monitorIndex, float xPercent, float yPercent) {

    auto monitors = GetMonitorsInfo();
    if (monitors.empty() || monitorIndex >= monitors.size()) {
        monitorIndex = 0;
    }

    MonitorInfo& monitor = monitors[monitorIndex];

    int xRelative = static_cast<int>((xPercent / 100.0f) * monitor.width);
    int yRelative = static_cast<int>((yPercent / 100.0f) * monitor.height);

    int absoluteX = monitor.left + min(xRelative, monitor.width - 1);
    int absoluteY = monitor.top + min(yRelative, monitor.height - 1);

    HDC hdc = GetDC(NULL);
    COLORREF color = GetPixel(hdc, absoluteX, absoluteY);
    ReleaseDC(NULL, hdc);

    return color;
}

bool getZonePixelsColorsOnMonitorPercent(int monitorIndex, float startXPercent, float startYPercent) {

    auto monitors = GetMonitorsInfo();
    if (monitors.empty() || monitorIndex >= monitors.size()) {
        monitorIndex = 0;
    }
    MonitorInfo& monitor = monitors[monitorIndex];

    int startXRelative = static_cast<int>((startXPercent / 100.0f) * monitor.width);
    int startYRelative = static_cast<int>((startYPercent / 100.0f) * monitor.height);

    int absX = monitor.left + startXRelative;
    int absY = monitor.top + startYRelative;

    HDC hdc = GetDC(NULL);

    for (int x = 0; x < 15; x += 3) {
        for (int y = 0; y < 15; y += 3) {
            COLORREF color = GetPixel(hdc, absX + x, absY + y);
            int r = GetRValue(color);
            int g = GetGValue(color);
            int b = GetBValue(color);

            if (r > 250 && g > 250 && b > 250) {
                ReleaseDC(NULL, hdc);
                return true;
            }
        }
    }

    ReleaseDC(NULL, hdc);
    return false;
}