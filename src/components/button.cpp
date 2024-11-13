#include "components/button.h"

Button::Button(HWND _parent, const wchar_t* _text, int _x, int _y, int _width, int _height, UINT _id)
    : parent(_parent), text(_text), x(_x), y(_y), width(_width), height(_height), id(_id) {}

void Button::create() {
    hwndButton = CreateWindow(
        L"BUTTON",      // Predefined class; the name of the button class
        text,           // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        x, y, width, height,  // Position and size
        parent,         // Parent window
        (HMENU)id,      // Control ID
        (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
        NULL);          // Additional data
}

HWND Button::getHWND() const {
    return hwndButton;
}
