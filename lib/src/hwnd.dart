import 'dart:async';
import 'dart:ffi';
import 'dart:math';
import 'dart:isolate';
import 'package:ffi/ffi.dart';

import 'package:win32/win32.dart';

/// Creates a new Win32 window on another [Isolate] and returns its handle as [int].
///
/// VLC video output is rendered on this window using `libvlc_media_player_set_hwnd`
/// and the window is embedded using
/// [flutter_native_view](https://github.com/alexmercerind/flutter_native_view.git).
///
/// TODO: hot-reload not working.
Future<int> createHWND() async {
  final receiver = ReceivePort();
  Isolate.spawn(_createWindowAndStartMessageLoop, receiver.sendPort);
  return await receiver.first as int;
}

/// Creates a new Win32 window on the current [Isolate] and starts the message loop.
void _createWindowAndStartMessageLoop(SendPort sendPort) {
  final lpStartupInfo = calloc<STARTUPINFO>();
  final hInstance = GetModuleHandle(nullptr);
  GetStartupInfo(lpStartupInfo);
  final className = TEXT('DART_VLC_WINDOW');
  final wc = calloc<WNDCLASS>()
    ..ref.style = CS_HREDRAW | CS_VREDRAW
    ..ref.lpfnWndProc = Pointer.fromFunction<WindowProc>(_windowProc, 0)
    ..ref.hInstance = hInstance
    ..ref.lpszClassName = className
    ..ref.hCursor = LoadCursor(NULL, IDC_ARROW)
    ..ref.hbrBackground = CreateSolidBrush(0);
  RegisterClass(wc);
  final windowCaption = TEXT('dart_vlc.${Random().nextInt(1 << 32)}');
  final hWnd = CreateWindowEx(
      0,
      className,
      windowCaption,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      nullptr);
  free(windowCaption);
  free(className);
  if (hWnd == 0) {
    final error = GetLastError();
    throw WindowsException(HRESULT_FROM_WIN32(error));
  }
  UpdateWindow(hWnd);
  sendPort.send(hWnd);
  final msg = calloc<MSG>();
  while (GetMessage(msg, hWnd, NULL, NULL) != 0) {
    TranslateMessage(msg);
    DispatchMessage(msg);
  }
  free(msg);
}

/// Window procedure.
int _windowProc(int hWnd, int uMsg, int wParam, int lParam) {
  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_PAINT:
      final ps = calloc<PAINTSTRUCT>();
      final hdc = BeginPaint(hWnd, ps);
      final rect = calloc<RECT>();
      GetClientRect(hWnd, rect);
      FillRect(hdc, rect, CreateSolidBrush(0));
      EndPaint(hWnd, ps);
      return 0;
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
