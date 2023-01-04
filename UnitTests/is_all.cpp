#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__is_all)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			Assert::AreEqual(str_is_all("14,2", '0'), false);
			Assert::AreEqual(str_is_all("00010", '0'), false);
			Assert::AreEqual(str_is_all("0", '0'), true);
			Assert::AreEqual(str_is_all("00,000", '0'), true);

			Assert::AreEqual(str_is_all("00,000", '1'), false);
			Assert::AreEqual(str_is_all("1,11", '1'), true);
		}
	};
}