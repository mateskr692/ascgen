#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__pad_space)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char num[100];

			strcpy_s(num, "1,   1");
			str_pad_space(num, '0');
			Assert::AreEqual(num, "1,0001");

			strcpy_s(num, "0,      ");
			str_pad_space(num, '1');
			Assert::AreEqual(num, "0,111111");

			strcpy_s(num, "110,011");
			str_pad_space(num, '0');
			Assert::AreEqual(num, "110,011");
		}
	};
}