#ifndef MONITOR_SOURCE_H
#define MONITOR_SOURCE_H
#include "keyboard_mouse_control.h"
#include <windows.h>
#include <thread>
#include <atomic>
#include <vector>

using namespace  std;

struct MonitorInfo {
    int left;
    int top;
    int width;
    int height;
    bool isPrimary;
};

vector<MonitorInfo> GetMonitorsInfo();
void moveMouseOnMonitor(int monitorIndex, int xRelative, int yRelative);
COLORREF getPixelColorOnMonitorPercent(int monitorIndex, float xPercent, float yPercent);
bool getZonePixelsColorsOnMonitorPercent(int monitorIndex, float startXPercent, float startYPercent);


#endif // MONITOR_SOURCE_H
