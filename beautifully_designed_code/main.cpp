#include "keyboard_mouse_control.h"
#include "monitor_source.h"
#include "support_func.h"

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

    int delay = 150;

    system("cls");

    cout << "Скрипт готов к работе. Нажмите F6 для старта/паузы." << endl;

    bool control_white_round;

    COLORREF control_pixel_one;
    COLORREF control_pixel_two;
    COLORREF control_pixel_three;
    COLORREF control_pixel_four;

    while(g_running){
        while(g_active){

            first_enters();

            control_white_round = getZonePixelsColorsOnMonitorPercent(targetMonitor, 40.469f, 21.111f);

            if (!control_white_round){
                sendKeyPress(VK_ESCAPE);
            }

            else{

                while (true){
                    if (getPixelColorOnMonitorPercent(targetMonitor, 40.469f, 21.111f) == RGB(255, 255, 255)){
                        break;
                    }
                }

                control_pixel_one = getPixelColorOnMonitorPercent(targetMonitor, 13.281f, 21.574f);
                control_pixel_two = getPixelColorOnMonitorPercent(targetMonitor, 13.281f, 40.278f);
                control_pixel_three = getPixelColorOnMonitorPercent(targetMonitor, 13.281f, 59.259f);
                control_pixel_four = getPixelColorOnMonitorPercent(targetMonitor, 13.281f, 78.241f);

                if (control_pixel_one != RGB(234, 222, 0)){

                    first_slots(delay);

                }

                else if (control_pixel_two != RGB(234, 222, 0) &&
                         control_pixel_two != RGB(29, 29, 29)){

                    second_slots(delay);

                }

                else if (control_pixel_three != RGB(234, 222, 0) &&
                         control_pixel_three != RGB(29, 29, 29)){
                    third_slots(delay);

                }

                else if (control_pixel_four != RGB(234, 222, 0) &&
                         control_pixel_four != RGB(29, 29, 29)){
                    fourth_slots(delay);

                }


            }

            sendKeyPress(VK_ESCAPE);

        }
    }

    return 0;

}
