#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__str_multiply)
	{
		public:
		TEST_METHOD(WholeMultiply)
		{
			char result[100];

			str_whole_multiply("23", result, 10, 16);
			Assert::AreEqual(result, "368");

			str_whole_multiply("23", result, 16, 10);
			Assert::AreEqual(result, "15E");

			str_whole_multiply("5123", result, 16, 0);
			Assert::AreEqual(result, "0");

			str_whole_multiply("0", result, 10, 16);
			Assert::AreEqual(result, "0");

			str_whole_multiply("101101", result, 2, 4);
			Assert::AreEqual(result, "10110100");
		}

		TEST_METHOD(DecimalMultiply)
		{
			char result[100];
			int carry;

			carry = str_decimal_multiply("23", result, 10, 4);
			Assert::AreEqual(result, "92");
			Assert::AreEqual(carry, 0);

			carry = str_decimal_multiply("9", result, 10, 4);
			Assert::AreEqual(result, "6");
			Assert::AreEqual(carry, 3);

			carry = str_decimal_multiply("00A", result, 16, 4);
			Assert::AreEqual(result, "028");
			Assert::AreEqual(carry, 0);

			carry = str_decimal_multiply("0", result, 10, 4);
			Assert::AreEqual(result, "0");
			Assert::AreEqual(carry, 0);

			carry = str_decimal_multiply("23", result, 10, 0);
			Assert::AreEqual(result, "00");
			Assert::AreEqual(carry, 0);

			carry = str_decimal_multiply("101", result, 2, 7);
			Assert::AreEqual(result, "011");
			Assert::AreEqual(carry, 4);
		}
	};
}