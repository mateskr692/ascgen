#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__is_greater)
	{
		public:
		TEST_METHOD(WholeTest)
		{
			Assert::AreEqual(str_is_greater("352", "301"), 1);
			Assert::AreEqual(str_is_greater("451", "9"), 1);
			Assert::AreEqual(str_is_greater("35671", "35671"), 0);
			Assert::AreEqual(str_is_greater("0005", "0031"), -1);
		}

		TEST_METHOD(DecimalTest)
		{
			Assert::AreEqual(str_decimal_is_greater("352", "301"), 1);
			Assert::AreEqual(str_decimal_is_greater("46", "9"), -1);
			Assert::AreEqual(str_decimal_is_greater("512", "512"), 0);
			Assert::AreEqual(str_decimal_is_greater("0061", "01"), -1);

		}
	};
}