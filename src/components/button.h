#ifndef COMPONENT_BUTTON_H
#define COMPONENT_BUTTON_H

#include <Windows.h>

class Button {
    public:
        Button(HWND _parent, const wchar_t* _text, int _x, int _y, int _width, int _height, UINT _id);
        void create();
        HWND getHWND() const;

    private:
        HWND parent;
        const wchar_t* text;
        int x, y, width, height;
        UINT id;
        HWND hwndButton;
};

#endif // COMPONENT_BUTTON_H