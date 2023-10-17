HWND CreateWin32Window(const wchar_t* title, int width, int height, bool fullscreen = false) {
    // Valores predeterminados
    const wchar_t* defaultTitle = L"Mi Ventana Win32";
    HFONT defaultFont = NULL; // Puedes establecer una fuente personalizada aquí
    HICON defaultIcon = LoadIcon(NULL, IDI_APPLICATION); // Ícono predeterminado
    HMENU defaultMenu = NULL; // Puedes configurar una barra de menú personalizada aquí

    // Utiliza los valores predeterminados si no se proporcionan valores personalizados
    if (title == nullptr) {
        title = defaultTitle;
    }
    if (width == 0 || height == 0) {
        width = GetSystemMetrics(SM_CXSCREEN); // Ancho de la pantalla
        height = GetSystemMetrics(SM_CYSCREEN); // Alto de la pantalla
    }

    // Resto de tu código para crear la ventana...

    // Si se selecciona el modo de pantalla completa
    if (fullscreen) {
        // Cambiar la resolución de la pantalla
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = 32; // Puedes ajustar esto según tu preferencia
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // No se pudo cambiar a pantalla completa, manejar el error
        }
    }

    // Crea una estructura WindowData con los valores predeterminados
    WindowData* windowData = new WindowData;
    windowData->hFont = defaultFont;
    windowData->hIcon = defaultIcon;
    windowData->hMenu = defaultMenu;

    // Asigna la estructura WindowData a la propiedad GWLP_USERDATA
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)windowData);

    // Resto de la creación de la ventana...

    return hwnd;
}
