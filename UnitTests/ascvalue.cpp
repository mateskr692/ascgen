#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(stdextension__ascvalue)
	{
	public:
		TEST_METHOD(char_to_asc)
		{
			Assert::AreEqual(char_to_ascvalue('1'), 1);
			Assert::AreEqual(char_to_ascvalue('5'), 5);
			Assert::AreEqual(char_to_ascvalue('A'), 10);
			Assert::AreEqual(char_to_ascvalue('a'), 10);
			Assert::AreEqual(char_to_ascvalue('F'), 15);
			Assert::AreEqual(char_to_ascvalue('Z'), 35);

			Assert::AreEqual(char_to_ascvalue('%'), -1);
			Assert::AreEqual(char_to_ascvalue('='), -1);
			Assert::AreEqual(char_to_ascvalue(','), -1);
		}

		TEST_METHOD(asc_to_char)
		{
			Assert::AreEqual(ascvalue_to_char(1), '1');
			Assert::AreEqual(ascvalue_to_char(5), '5');
			Assert::AreEqual(ascvalue_to_char(10), 'A');
			Assert::AreEqual(ascvalue_to_char(15), 'F');
			Assert::AreEqual(ascvalue_to_char(35), 'Z');
		}
	};
}