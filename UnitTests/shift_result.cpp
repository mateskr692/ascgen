#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(shared__shift_result)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char num[100];

			strcpy_s(num, "0,101001");
			mathmult_shift_result(num, 4);
			Assert::AreEqual(num, "0.1010,01");

			strcpy_s(num, "0,101001");
			mathmult_shift_result(num, 1);
			Assert::AreEqual(num, "0.1,01001");

			strcpy_s(num, "0,101001");
			mathmult_shift_result(num, 0);
			Assert::AreEqual(num, "0.0,101001");

			strcpy_s(num, "1,101001");
			mathmult_shift_result(num, 0);
			Assert::AreEqual(num, "1.1,101001");

		}
	};
}