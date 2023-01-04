#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__int_to_str)
	{
		public:
		TEST_METHOD(ValidResult)
		{
			char result[100];

			int_to_str(result, 351, 10);
			Assert::AreEqual(result, "351");

			int_to_str(result, 0, 10);
			Assert::AreEqual(result, "0");

			int_to_str(result, 34, 2);
			Assert::AreEqual(result, "100010");

			int_to_str(result, 34, 16);
			Assert::AreEqual(result, "22");
		}

	};
}