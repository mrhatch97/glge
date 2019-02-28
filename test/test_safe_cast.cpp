#include <glge/Util/Util.h>

using namespace eng::util;

int main()
{
	safe_cast<float, double>(3.14f);

	safe_cast<float, float>(3.14f);

	try
	{
		safe_cast<double, int>(3.14);
		return -1;
	}
	catch (const std::logic_error &)
	{ }

	try
	{
		safe_cast<double, float>(3.14);
		return -1;
	}
	catch (const std::logic_error &)
	{ }

	return 0;
}