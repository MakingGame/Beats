#ifndef GUI_LAYOUT_VSCROLL_LAYOUT_H__INCLUDE
#define GUI_LAYOUT_VSCROLL_LAYOUT_H__INCLUDE

#include "ScrollLayout.h"

namespace FCGUI
{
    class VScrollLayout : public ScrollLayout
    {
    public:
        VScrollLayout(size_t colCount = 1);
    };
}

#endif