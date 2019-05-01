#include <glge/support/event/input/input_dispatcher.h>

#include "test_utils.h"

using namespace glge::event::input;

void test_single_key()
{
}

void test_repeat_key()
{
}

void test_mouse_button()
{
}

void test_mouse_move()
{
}

void test_mouse_drag()
{
}

void test_mouse_scroll()
{
}

int main()
{
	Test(test_single_key).run();
	Test(test_repeat_key).run();
	Test(test_mouse_button).run();
	Test(test_mouse_move).run();
  Test(test_mouse_drag).run();
  Test(test_mouse_scroll).run();
}
