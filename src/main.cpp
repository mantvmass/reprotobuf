#include <iostream>
#include <Windows.h>
#include "core/context.h"
#include "components/button.h"


// ฟังก์ชันที่ใช้ในการแสดงข้อความกลางหน้าจอ
void DrawCenteredText(HWND hwnd, const char* text, HDC hdc) {
    RECT rect;
    GetClientRect(hwnd, &rect);  // รับขนาดของพื้นที่หน้าต่าง

    // คำนวณตำแหน่งของข้อความให้อยู่กลาง
    DrawTextA(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// ฟังก์ชัน Window Procedure (WndProc) สำหรับจัดการข้อความ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND: {
        // ตรวจสอบว่าเป็นการคลิกที่ปุ่มหรือไม่
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case 1: // ปุ่มที่มี ID 1
            std::cout << "BUTTON CLICK" << std::endl;
            break;
        case 11: // ตัวอย่างเมนูที่มี ID 11
            std::cout << "Option File Selected" << std::endl;
            break;
        case 21: // ตัวอย่างเมนูที่มี ID 21
            std::cout << "Option Edit Selected" << std::endl;
            break;
        case 33: // ตัวอย่างเมนูที่มี ID 33
            std::cout << "Option Help Selected" << std::endl;
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        {
            // ใช้ BeginPaint และ EndPaint สำหรับการวาดที่ถูกต้อง
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);  // เริ่มการวาด
            DrawCenteredText(hwnd, ctx -> test.c_str(), hdc);  // วาดข้อความ
            EndPaint(hwnd, &ps);  // สิ้นสุดการวาด
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// ฟังก์ชันหลักของโปรแกรม
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    #ifdef _DEBUG
        AllocConsole(); // เปิดคอนโซลในโหมด Debug
        FILE* pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout); // ใช้เพื่อเชื่อมต่อคอนโซลที่สร้างใหม่กับ stdout เพื่อให้สามารถแสดงข้อความจาก std::cout ได้.
        std::cout << "srtyhsrth" << std::endl;
    #endif

    // กำหนดค่า WNDCLASS
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWin32App";

    // ลงทะเบียนคลาส
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Failed to register window class", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // สร้างหน้าต่างหลัก
    HWND hwnd = CreateWindowEx(
        0,                        // ขยายสไตล์ (ใช้ค่าเริ่มต้น)
        L"MyWin32App",            // ชื่อคลาส
        L"Hello, Win32!",         // ชื่อหน้าต่าง
        WS_OVERLAPPEDWINDOW,      // สไตล์หน้าต่าง
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // ขนาดหน้าต่าง
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, L"Failed to create window", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // สร้างเมนูหลัก
    HMENU hMenu = CreateMenu();

    // สร้างเมนูแบบ DropDown
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, 11, L"File");
    AppendMenu(hFileMenu, MF_STRING, 21, L"Edit");
    AppendMenu(hFileMenu, MF_STRING, 33, L"Help");

    // เพิ่มเมนูแบบ DropDown ลงในเมนูหลัก
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"Menu");

    // ตั้งค่าเมนูให้กับหน้าต่าง
    SetMenu(hwnd, hMenu);

    Button button(hwnd, L"Click Me", 150, 100, 100, 30, 1);  // ปุ่ม
    button.create();  // สร้างปุ่ม

    // แสดงหน้าต่าง
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // วนลูปข้อความ
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
