#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__str_divide)
	{
	public:
		TEST_METHOD(WholeDivideConstant)
		{
			char result[100];
			int reminder;

			reminder = str_whole_divide_constant("53", result, 10, 8);
			Assert::AreEqual(result, "6");
			Assert::AreEqual(reminder, 5);

			reminder = str_whole_divide_constant("461A", result, 16, 10);
			Assert::AreEqual(result, "702");
			Assert::AreEqual(reminder, 6);

			reminder = str_whole_divide_constant("8", result, 10, 16);
			Assert::AreEqual(result, "0");
			Assert::AreEqual(reminder, 8);

			reminder = str_whole_divide_constant("101001", result, 2, 10);
			Assert::AreEqual(result, "100");
			Assert::AreEqual(reminder, 1);
		}

		TEST_METHOD(WholeDivideValue)
		{
			int result;

			result = str_whole_divide_string("163", "42", 10);
			Assert::AreEqual(result, 3);

			result = str_whole_divide_string("64AC", "B9", 16);
			Assert::AreEqual(result, 139);

			result = str_whole_divide_string("0", "16", 10);
			Assert::AreEqual(result, 0);

			result = str_whole_divide_string("101101", "1011", 2);
			Assert::AreEqual(result, 4);
		}

		TEST_METHOD(DecimalDivideValue)
		{
			int result;
			
			result = str_decimal_divide_string("68", "02", 10);
			Assert::AreEqual(result, 34);

			result = str_decimal_divide_string("68", "2", 10);
			Assert::AreEqual(result, 3);

			result = str_whole_divide_string("3B9", "04A", 16);
			Assert::AreEqual(result, 12);

			result = str_decimal_divide_string("101101", "00011", 2);
			Assert::AreEqual(result, 7);
		}
	};
}