#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__trim_leading)
	{
	public:
		TEST_METHOD(ValidResult)
		{
			char res[100];

			str_trim_leading("0014,12", res, '0');
			Assert::AreEqual(res, "14,12");

			str_trim_leading("0014,12", res, '1');
			Assert::AreEqual(res, "0014,12");

		}
	};
}