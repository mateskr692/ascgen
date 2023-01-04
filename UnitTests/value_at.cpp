#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__value_at)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char str[] = "124,B01";

			Assert::AreEqual(str_value_at(str, 0), 1);
			Assert::AreEqual(str_value_at(str, 1), 2);
			Assert::AreEqual(str_value_at(str, 2), 4);

			Assert::AreEqual(str_value_at(str, 4), 11);
			Assert::AreEqual(str_value_at(str, 7), 0);
			Assert::AreEqual(str_value_at(str, 20), 0);
		}
	};
}