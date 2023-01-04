#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(shared__shift_left)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char num[100];

			strcpy_s(num, "0.101,01");
			mathmult_shift_left(num, 3);
			Assert::AreEqual(num, "0,10101");

			strcpy_s(num, "0.1,01");
			mathmult_shift_left(num, 1);
			Assert::AreEqual(num, "0,101");

			strcpy_s(num, "0.0,01");
			mathmult_shift_left(num, 0);
			Assert::AreEqual(num, "0,01");

		}
	};
}