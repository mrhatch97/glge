#include <glge/util/util.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::util;

void test_basic()
{
  bool enter = false, exit = false;

  {
    UniqueHandle handle(
        [&] { enter = true; },
        [&] { exit = true; }
    );
    test_assert(handle);
    test_assert(enter);
    test_assert(!exit);
  }
  test_assert(exit);
}

void test_chain()
{
  int enter = 0, exit = 0;

  {
    UniqueHandle handle(
        [&] { test_equal(0, enter++); },
        [&] { test_equal(1, exit++); }
    );

    test_equal(1, enter);
    test_equal(0, exit);

    handle.chain(
        [&] { test_equal(1, enter++); },
        [&] { test_equal(0, exit++); }
    );

    test_equal(2, enter);
    test_equal(0, exit);
  }

  test_equal(2, exit);
}

void test_reset()
{
  bool enter = false, exit = false;

  UniqueHandle handle(
      [&] { enter = true; },
      [&] { exit = true; }
  );

  handle.reset();

  test_assert(exit);
  test_assert(!handle);
}

void test_reset_f()
{
  bool enter = false, exit = false, exit_2 = false;

  UniqueHandle handle(
      [&] { enter = true; },
      [&] { exit = true; }
  );

  handle.reset([&] { exit_2 = true; });

  test_assert(exit);
  test_assert(handle);

  handle.reset();

  test_assert(exit_2);
}

void test_release()
{
  bool enter = false, exit = false;

  UniqueHandle handle(
      [&] { enter = true; },
      [&] { exit = true; }
  );

  handle.release();

  test_assert(!exit);
  test_assert(!handle);
}

void test_move()
{
  bool enter = false, exit = false;

  {
    UniqueHandle handle_1;

    {
      UniqueHandle handle_2(
          [&] { enter = true; },
          [&] { exit = true; }
      );

      handle_1 = std::move(handle_2);
    }
    test_assert(!exit);
  }
  test_assert(exit);
}

int main()
{
	Test(test_basic).run();
	Test(test_chain).run();
  Test(test_reset).run();
  Test(test_reset_f).run();
  Test(test_release).run();
  Test(test_move).run();
}
