#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__str_to_int)
	{
		TEST_METHOD(ValidNumber)
		{
			result res;
			int num;

			res = str_to_int("53", &num, 10);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num, 53);

			res = str_to_int("22", &num, 16);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num, 0x22);

			res = str_to_int("10010", &num, 2);
			Assert::IsFalse(res.is_error);
			Assert::AreEqual(num, 0b10010);
		}

		TEST_METHOD(EmptyNumber)
		{
			result res;
			int num;

			res = str_to_int("", &num, 10);
			Assert::IsTrue(res.is_error);
		}

		TEST_METHOD(InvalidSymbol)
		{
			result res;
			int num;

			res = str_to_int("474&1", &num, 10);
			Assert::IsTrue(res.is_error);
		}

		TEST_METHOD(InvalidBase)
		{
			result res;
			int num;

			res = str_to_int("1523", &num, 4);
			Assert::IsTrue(res.is_error);
		}

	};
}