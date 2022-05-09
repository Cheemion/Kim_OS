#include "../../include/gui/widget.h"
using namespace myos::gui;
using namespace myos::common;



Widget::Widget(Widget* parent,
		common::int32_t x,
		common::int32_t y,
		common::int32_t w,
		common::int32_t h,
		common::uint8_t r,
		common::uint8_t g,
		common::uint8_t b
	       ){
    this->parent =  parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->Focussable = true;
}

Widget::~Widget() {}


void Widget::GetFocus(Widget* widget){
    if(parent != 0)
	parent->GetFocus(widget);
}
void Widget::ModelToScreen(common::int32_t& x, common::int32_t& y){
    if(parent != 0)
	parent->ModelToScreen(x, y);
    x += this->x;
    y += this->y;
}
void Widget::Draw(GraphicsContext* gc){
    int X = 0;
    int Y = 0;
    ModelToScreen(X, Y);
    gc->FillRectangle(X, Y, w, h, r, g, b);
}
void Widget::OnMouseDown(common::int32_t x, common::int32_t y){
    if(Focussable)
	GetFocus(this);
}
void Widget::OnMouseUp(common::int32_t x, common::int32_t y){
    
}
void Widget::OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy){
    
}
void Widget::OnKeyDown(common::int32_t x, common::int32_t y){}
void Widget::OnKeyUp(common::int32_t x, common::int32_t y){}




 
