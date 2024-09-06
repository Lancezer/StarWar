#include "../include/message.h"

static MOUSEMESSAGE mouse;

void peek_mouse_message()
{
	ExMessage msg;
	mouse.is_up = false;
	mouse.is_down = false;
	while (peekmessage(&msg, EX_MOUSE))
	{
		if (msg.message == WM_MOUSEMOVE)
		{
			mouse.pos.x = msg.x;
			mouse.pos.y = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP)
		{
			mouse.is_up = true;
			mouse.is_down = false;
		}
		else if (msg.message == WM_LBUTTONDOWN)
		{
			mouse.is_up = false;
			mouse.is_down = true;
		}
	}
}

pMOUSEMESSAGE get_mouse_message()
{
	return &mouse;
}

bool is_mouse_in_rect(COORDINATE mouse,RECT r)
{
	if (r.left < mouse.x && mouse.x < r.right && r.bottom > mouse.y && mouse.y > r.top)
	{
		return true;
	}
	return false;
}

bool is_key_down(int key) 
{
	return (GetAsyncKeyState(key) & 0x8000);
}