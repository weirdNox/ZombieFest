#include "zfest_common.h"
#include "zfest_memory.h"
#include "zfest_math.h"
#include "zfest_platform.h"
#include "zfest.h"

#include "zfest_render.cpp"
#include "zfest.cpp"

#include <windows.h>
#include <gl/gl.h>

#include "zfest_opengl.cpp"

internal inline uint64 win32GetWallClock()
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result.QuadPart;
}

internal inline real32 win32GetElapsedSeconds(uint64 begin, uint64 end, uint64 clockFrequency)
{
    real32 result = (real32)(end - begin) / (real32)clockFrequency;
    return result;
}

internal LRESULT CALLBACK win32MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    Core *core;
    if(message == WM_CREATE)
    {
        CREATESTRUCT *createStruct = (CREATESTRUCT *)lParam;
        core = (Core *)createStruct->lpCreateParams;
        SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)core);
    }
    else
    {
        core = (Core *)GetWindowLongPtr(window, GWLP_USERDATA);
    }

    switch(message)
    {
        case WM_KEYDOWN:
        {
            core->keyStates[wParam] = true;
            core->keyDeltas[wParam] = !(lParam & (1<<30));

            return 0;
        } break;

        case WM_KEYUP:
        {
            core->keyStates[wParam] = false;
            core->keyDeltas[wParam] = true;

            return 0;
        } break;

        case WM_LBUTTONDOWN:
        {
            core->keyStates[LEFT_MOUSE_BUTTON] = true;
            core->keyDeltas[LEFT_MOUSE_BUTTON] = true;

            return 0;
        } break;

        case WM_LBUTTONUP:
        {
            core->keyStates[LEFT_MOUSE_BUTTON] = false;

            return 0;
        } break;

        case WM_SIZE:
        {
            core->width = LOWORD(lParam);
            core->height = HIWORD(lParam);
        } break;

        case WM_CLOSE:
        case WM_DESTROY:
        {
            core->running = false;
        } break;
    }

    return DefWindowProcA(window, message, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    Core core = {};

    // NOTE(nox): Open window
    WNDCLASSA windowClass = {};
    windowClass.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    windowClass.lpfnWndProc = win32MainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    windowClass.lpszClassName = "ZFestWindowClass";

    if(!RegisterClassA(&windowClass))
    {
        OutputDebugStringA("Could not register window class.");
        return 1;
    }

    DWORD windowStyles = WS_VISIBLE|WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
    RECT windowRect = {0, 0, 1024, 576};
    AdjustWindowRect(&windowRect, windowStyles, 0);
    HWND window = CreateWindowA(windowClass.lpszClassName,
                                "Zombie Fest",
                                windowStyles,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                windowRect.right - windowRect.left,
                                windowRect.bottom - windowRect.top,
                                0,
                                0,
                                instance,
                                &core);

    if(!window)
    {
        OutputDebugStringA("Could not open window.");
        return 1;
    }

    // NOTE(nox): Enable OpenGL
    HDC deviceContext = GetDC(window);

    PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {};
    pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
    pixelFormatDescriptor.nVersion = 1;
    pixelFormatDescriptor.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDescriptor.cColorBits = 32;

    int32 pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
    if(pixelFormat == 0)
    {
        OutputDebugStringA("Error on choosing pixel format.");
        return 0;
    }

    if(!SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor))
    {
        OutputDebugStringA("Error on setting pixel format.");
        return 0;
    }
    DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor);

    HGLRC glRenderingContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, glRenderingContext);

    // NOTE(nox): Memory
    MemSize pushBufferSize = megabytes(64);
    void *pushBuffer = VirtualAlloc(0, pushBufferSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    core.persistentMemorySize = megabytes(256);
    core.transientMemorySize = megabytes(512);
    MemSize totalMemorySize = core.persistentMemorySize + core.transientMemorySize;
    void *baseAddress = (void *)terabytes(2);
    core.persistentMemory = VirtualAlloc(baseAddress, totalMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    core.transientMemory = (uint8 *)core.persistentMemory + core.persistentMemorySize;

    if(!pushBuffer || !core.persistentMemory || !core.transientMemory)
    {
        OutputDebugStringA("Error allocating memory.");
        return 0;
    }

    core.renderCommands.base = (uint8 *)pushBuffer;
    core.renderCommands.maxSize = pushBufferSize;

    core.frameTime = (1.0f/60.0f);

    LARGE_INTEGER clockFrequency;
    QueryPerformanceFrequency(&clockFrequency);

    uint64 beginFrameTime = win32GetWallClock();

    core.running = true;
    while(core.running)
    {
        zeroArray(core.keyDeltas);
        core.renderCommands.size = 0;

        MSG message;
        while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }

        POINT mousePoint;
        GetCursorPos(&mousePoint);
        ScreenToClient(window, &mousePoint);
        core.mousePos = v2i(mousePoint.x, core.height - mousePoint.y);

        gameUpdateAndRender(&core);

        renderCommands(&core.renderCommands, core.width, core.height);
        SwapBuffers(deviceContext);
        // TODO(nox): Sound

        uint64 now = win32GetWallClock();
        real32 elapsedSeconds = win32GetElapsedSeconds(beginFrameTime, now, clockFrequency.QuadPart);

        if(elapsedSeconds < core.frameTime)
        {
            // FIXME(nox): Sleep not working... need a higher resolution one!
            // DWORD sleepMs = (DWORD)(1000.f * (core.frameTime - elapsedSeconds));
            // if(sleepMs > 10)
            // {
            //     Sleep(sleepMs);
            //     now = win32GetWallClock();
            //     elapsedSeconds = win32GetElapsedSeconds(beginFrameTime, now, clockFrequency.QuadPart);
            // }

            while(core.frameTime - elapsedSeconds > 0.00005f)
            {
                now = win32GetWallClock();
                elapsedSeconds = win32GetElapsedSeconds(beginFrameTime, now, clockFrequency.QuadPart);
            }

            if(elapsedSeconds - core.frameTime > 0.00001f)
            {
                OutputDebugStringA("Missed frameTime!\n");
            }
        }

        beginFrameTime = now;

        core.time += core.frameTime;
    }

    return 0;
}
