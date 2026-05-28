#ifndef SUPPORT_FUNC_H
#define SUPPORT_FUNC_H

#include <atomic>


extern std::atomic<bool> g_active;
extern std::atomic<bool> g_running;

void KeyboardHook();
void first_enters();
void first_slots(const int& delay);
void second_slots(const int& delay);
void third_slots(const int& delay);
void fourth_slots(const int& delay);

#endif // SUPPORT_FUNC_H
