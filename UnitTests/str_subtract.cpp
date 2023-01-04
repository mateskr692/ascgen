#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__subtract)
	{
		public:
		TEST_METHOD(WholeTest)
		{
			char result[100];

			str_whole_subtract("5251", "916", result, 10);
			Assert::AreEqual(result, "4335");

			str_whole_subtract("721", "719", result, 10);
			Assert::AreEqual(result, "2");

			str_whole_subtract("6AC9A", "24AAF", result, 16);
			Assert::AreEqual(result, "461EB");

			str_whole_subtract("5A", "5A", result, 16);
			Assert::AreEqual(result, "0");

			str_whole_subtract("110101", "1011", result, 2);
			Assert::AreEqual(result, "101010");
		}

		TEST_METHOD(DecimalTest)
		{
			char result[100];

			str_decimal_subtract("684", "4", result, 10);
			Assert::AreEqual(result, "284");

			str_decimal_subtract("00D2", "0009", result, 16);
			Assert::AreEqual(result, "00C9");

			str_decimal_subtract("D5A", "D5A", result, 16);
			Assert::AreEqual(result, "000");

			str_decimal_subtract("01101", "01001", result, 2);
			Assert::AreEqual(result, "00100");
		}

	};
}