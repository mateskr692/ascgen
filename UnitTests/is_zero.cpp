#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__is_zero)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			Assert::AreEqual(str_is_zero("14,2"), false);
			Assert::AreEqual(str_is_zero("00010"), false);
			Assert::AreEqual(str_is_zero("0"), true);
			Assert::AreEqual(str_is_zero("0000000"), true);
			Assert::AreEqual(str_is_zero("00,000"), true);
		}
	};
}