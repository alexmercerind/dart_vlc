#include "include/dart_vlc/win32_window.h"

#include <future>
#include <random>
#include <string>
#include <thread>

constexpr auto kWindowClassName = L"DART_VLC_WINDOW";

HWND CreateWin32Window(bool fullscreen) {
  std::promise<HWND> window_promise;
  std::thread([fullscreen, &window_promise]() {
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

    int screenWidth, screenHeight;
    if (fullscreen) {
      screenWidth = GetSystemMetrics(SM_CXSCREEN);
      screenHeight = GetSystemMetrics(SM_CYSCREEN);
    } else {
      screenWidth = 800;  // Ancho predeterminado
      screenHeight = 600; // Alto predeterminado
    }

    HWND window = CreateWindowEx(
        0, kWindowClassName, L"DART_VLC_WINDOW", fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW,
        0, 0, screenWidth, screenHeight, NULL, NULL, instance, nullptr);

    if (fullscreen) {
      DEVMODE dmScreenSettings;
      memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
      dmScreenSettings.dmSize = sizeof(dmScreenSettings);
      dmScreenSettings.dmPelsWidth = screenWidth;
      dmScreenSettings.dmPelsHeight = screenHeight;
      dmScreenSettings.dmBitsPerPel = 32; // Puedes ajustar esto según tu preferencia
      dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

      if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        // No se pudo cambiar a pantalla completa, manejar el error
      }
    }

    window_promise.set_value(window);

    MSG msg;
    while (::GetMessage(&msg, window, 0, 0)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }

    // Restaurar la configuración de pantalla original al cerrar la ventana
    if (fullscreen) {
      ChangeDisplaySettings(nullptr, 0);
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
