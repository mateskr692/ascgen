#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__whole_convert)
	{
		public:
		TEST_METHOD(ValidResult)
		{
			char result[100];

			str_whole_convert("3468", result, 10, 16);
			Assert::AreEqual(result, "D8C");

			str_whole_convert("3468", result, 10, 2);
			Assert::AreEqual(result, "110110001100");

			str_whole_convert("0", result, 8, 4);
			Assert::AreEqual(result, "0");
		}
	};
}