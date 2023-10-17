// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#include "include/dart_vlc/win32_window.h"

#include <future>
#include <random>
#include <string>
#include <thread>

constexpr auto kWindowClassName = L"DART_VLC_WINDOW";

HWND CreateWin32Window() {
  std::promise<HWND> window_promise;
  std::thread([&]() {
    auto startup_info = STARTUPINFO{};
    ::GetStartupInfo(&startup_info);
    auto instance = ::GetModuleHandle(nullptr);
    auto window_class = WNDCLASS{};
    window_class.lpszClassName = kWindowClassName;
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = Win32WindowProc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = instance;
    window_class.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
    window_class.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    window_class.hbrBackground =
        static_cast<HBRUSH>(::CreateSolidBrush(RGB(0, 0, 0)));
    ::RegisterClass(&window_class);
    std::random_device random_device;
    std::mt19937 range(random_device());
    std::uniform_int_distribution<int> uniform(0, INT32_MAX);
    HWND window = CreateWindowEx(
        0, kWindowClassName,
        (L"dart_vlc.instance." + std::to_wstring(uniform(range))).c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, NULL, NULL, instance, nullptr);
    window_promise.set_value(window);
    MSG msg;
    while (::GetMessage(&msg, window, 0, 0)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }).detach();
  return window_promise.get_future().get();
}

LRESULT CALLBACK Win32WindowProc(HWND window, UINT message, WPARAM wparam,
                                 LPARAM lparam) {
  switch (message) {
    case WM_DESTROY: {
      ::PostQuitMessage(0);
      return 0;
    }
  }
  return DefWindowProc(window, message, wparam, lparam);
}
