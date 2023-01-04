#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__strcat_B)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char str[100];

			strcpy_s(str, "124,B01");
			strcat_b(str, "41");
			Assert::AreEqual(str, "41124,B01");

			strcpy_s(str, "101,01");
			strcat_b(str, "000");
			Assert::AreEqual(str, "000101,01");

			strcpy_s(str, "4,3");
			strcat_b(str, "");
			Assert::AreEqual(str, "4,3");

		}
	};
}