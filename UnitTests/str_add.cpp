#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__add)
	{
	public:
		TEST_METHOD(AddValue)
		{
			char result[100];
			int carry;

			carry = str_addnum("01101,110", "10001,001", result, 2);
			Assert::AreEqual(result, "11110,111");
			Assert::AreEqual(carry, 0);

			carry = str_addnum("1101,10", "1011,11", result, 2);
			Assert::AreEqual(result, "1001,01");
			Assert::AreEqual(carry, 1);

			carry = str_addnum("1.1011010,0111", "1.1010010,1010", result, 2);
			Assert::AreEqual(result, "1.0101101,0001");
			Assert::AreEqual(carry, 1);
		}

		TEST_METHOD(AddConstant)
		{
			char result[100];
			int carry;

			carry = str_addconst("01101,110", result, 2, 1);
			Assert::AreEqual(result, "01101,111");
			Assert::AreEqual(carry, 0);

			carry = str_addconst("11,0000", result, 2, 3);
			Assert::AreEqual(result, "11,0011");
			Assert::AreEqual(carry, 0);

			carry = str_addconst("1.11,110", result, 2, 2);
			Assert::AreEqual(result, "0.00,000");
			Assert::AreEqual(carry, 1);
		}
	};
}