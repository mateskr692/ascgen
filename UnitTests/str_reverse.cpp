#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__str_reverse)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char str[100];

			strcpy_s(str, "301");
			str_reverse(str);
			Assert::AreEqual(str, "103");

			strcpy_s(str, "123456");
			str_reverse(str);
			Assert::AreEqual(str, "654321");

		}
	};
}