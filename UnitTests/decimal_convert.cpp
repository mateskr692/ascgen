#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__decimal_convert)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char result[100];

			str_decimal_convert("375", result, 10, 2);
			Assert::AreEqual(result, "011");

			str_decimal_convert("A", result, 16, 10);
			Assert::AreEqual(result, "625");

			str_decimal_convert("5", result, 10, 16);
			Assert::AreEqual(result, "8");

			str_decimal_convert("0", result, 8, 4);
			Assert::AreEqual(result, "0");
		}
	};
}