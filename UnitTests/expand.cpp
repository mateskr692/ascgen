#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__expand)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char num[100];

			strcpy_s(num, "1,25");
			str_expand(num, 3, 5, '0');
			Assert::AreEqual(num, "001,25000");

			strcpy_s(num, "1,01");
			str_expand(num, 5, 0, '1');
			Assert::AreEqual(num, "11111,01");

			strcpy_s(num, "1,01");
			str_expand(num, 0, 2, '0');
			Assert::AreEqual(num, "1,01");

			strcpy_s(num, "1,01");
			str_expand(num, 0, 5, '0');
			Assert::AreEqual(num, "1,01000");
		}
	};
}