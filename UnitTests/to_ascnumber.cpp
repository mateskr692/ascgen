#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__to_ascnumber)
	{
		public:
		TEST_METHOD(ValidNumber)
		{
			ascnumber num = {};
			result res = {};
				
			res = str_to_ascnumber("14521,7923", &num);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num.whole, "14521");
			Assert::AreEqual(num.decimal, "7923");
			Assert::AreEqual(num.is_negative, false);
			Assert::AreEqual(num.highest_digit, char_to_ascvalue('9'));
			ascnumber_clean(&num);

			res = str_to_ascnumber("-4B31C,4A", &num);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num.whole, "4B31C");
			Assert::AreEqual(num.decimal, "4A");
			Assert::AreEqual(num.is_negative, true);
			Assert::AreEqual(num.highest_digit, char_to_ascvalue('C'));
			ascnumber_clean(&num);

			res = str_to_ascnumber("1.89fa1", &num);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num.whole, "89fa1");
			Assert::IsNull(num.decimal);
			Assert::AreEqual(num.is_negative, true);
			Assert::AreEqual(num.highest_digit, char_to_ascvalue('f'));
			ascnumber_clean(&num);

			res = str_to_ascnumber("0.101101,a8icZp", &num);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num.whole, "101101");
			Assert::AreEqual(num.decimal, "a8icZp");
			Assert::AreEqual(num.is_negative, false);
			Assert::AreEqual(num.highest_digit, char_to_ascvalue('z'));
			ascnumber_clean(&num);
		}

		TEST_METHOD(InvalidSymbol)
		{
			ascnumber num = {};
			result res = {};
			
			res = str_to_ascnumber("1873,da^f", &num);
			Assert::IsTrue(res.is_error);

			res = str_to_ascnumber("%1873,da2f", &num);
			Assert::IsTrue(res.is_error);
		}

		TEST_METHOD(InvalidSign)
		{
			ascnumber num = {};
			auto res = str_to_ascnumber("2.1873,da2f", &num);
			Assert::IsTrue(res.is_error);
		}

		TEST_METHOD(MissingWhole)
		{
			ascnumber num = {};
			auto res = str_to_ascnumber(",da0f", &num);
			Assert::IsTrue(res.is_error);
		}

		TEST_METHOD(EmptyNumber)
		{
			ascnumber num = {};
			auto res = str_to_ascnumber("", &num);
			Assert::IsTrue(res.is_error);
		}
	};


}
