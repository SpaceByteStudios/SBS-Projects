#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void MakeWindowClickThrough(void* hwnd_ptr) {
  HWND hwnd = (HWND)hwnd_ptr;
  LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT);
}
