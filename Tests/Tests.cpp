#include <string>
#include <iostream>
#include "CppUnitTest.h"
#include "../coding_algorithms/Shannon_Fano.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		Shannon_Fano* coder;

		TEST_METHOD_CLEANUP(clean)
		{
			delete coder;
		}

		TEST_METHOD(test_get_number_def_letter)
		{
			std::string str1 = "LETI";
			std::string str2 = "LETI FKTI";

			try
			{
				coder = new Shannon_Fano(str1);
				Assert::AreEqual((size_t)4, coder->get_number_def_letter());
				delete coder;
				
				coder = new Shannon_Fano(str2);
				Assert::AreEqual((size_t)7, coder->get_number_def_letter());
			}
			catch (std::bad_alloc error)
			{
				// If there is a memory allocation error, always fail the test
				Assert::AreEqual("", error.what());
			}
		}

		TEST_METHOD(test_get_all_codes1)
		{
			std::string str = "LETI FKTI";
			std::string arr[] = { "001", "010", "100", "011", "000", "11", "101" };

			try
			{
				coder = new Shannon_Fano(str);
				std::string* res = coder->get_all_codes();

				for (size_t i = 0; i < 7; ++i)
					Assert::AreEqual(arr[i], res[i]);
			}
			catch (std::bad_alloc error)
			{
				// If there is a memory allocation error, always fail the test
				Assert::AreEqual("", error.what());
			}
		}

		TEST_METHOD(test_get_the_code)
		{
			std::string str = "LETI FKTI";

			coder = new Shannon_Fano(str);

			Assert::AreEqual((std::string)"000", coder->get_the_code('L'));
			Assert::AreEqual((std::string)"11", coder->get_the_code('I'));
			Assert::AreEqual((std::string)"100", coder->get_the_code('K'));
		}

		TEST_METHOD(test_get_all_codes)
		{
			std::string str = "LETI FKTI";
			std::string abs_res[] = { "001", "010", "100", "011", "000", "11", "101" };

			try
			{
				coder = new Shannon_Fano(str);
				std::string* function_res = coder->get_all_codes();

				for (size_t i = 0; i < 7; ++i)
					Assert::AreEqual(abs_res[i], function_res[i]);
			}
			catch (std::bad_alloc error)
			{
				// If there is a memory allocation error, always fail the test
				Assert::AreEqual("", error.what());
			}
		}

		TEST_METHOD(test_string_encoding_decoding)
		{
			std::string s[] = { "There are only two ways to live your life. One is as though nothing is a miracle. The other is as though everything is a miracle.",
								"And, when you want something, all the universe conspires in helping you to achieve it.",
								"Promise me you’ll always remember: You’re braver than you believe, and stronger than you seem, and smarter than you think.",
								"Your life is not a problem to be solved but a gift to be opened.", 
								"All our dreams can come true, if we have the courage to pursue them.", 
								"Times and conditions change so rapidly that we must keep our aim constantly focused on the future.", 
								"When you do something noble and beautiful and nobody noticed, do not be sad. For the sun every morning is a beautiful spectacle and yet most of the audience still sleeps.", 
								"As long as man continues to be the ruthless destroyer of lower living beings he will never know health or peace. For as long as men massacre animals, they will kill each other.", 
								"For true love is inexhaustible; the more you give, the more you have. And if you go to draw at the true fountainhead, the more water you draw, the more abundant is its flow.", 
								"Be soft. Do not let the world make you hard. Do not let pain make you hate. Do not let the bitterness steal your sweetness. Take pride that even though the rest of the world may disagree, you still believe it to be a beautiful place" };

			try
			{
				for (size_t i = 0; i < 10; ++i)
				{
					std::string current = s[i];
					coder = new Shannon_Fano(s[i]);

					Assert::AreEqual(current, coder->string_decoding(coder->string_encoding()));
					
					if (i != 9)
						delete coder;
				}
			}
			catch (std::bad_alloc error)
			{
				std::cerr << error.what() << std::endl;
			}
		}

		TEST_METHOD(test_encoding_result)
		{
			std::string s = "If you look at what you have in life, you’ll always have more. If you look at what you don’t have in life, you’ll never have enough";
			size_t abs_result[] = { 655, 537, 18 };

			coder = new Shannon_Fano(s);

			size_t* result_function = coder->encoding_result();

			for (size_t i = 0; i < 3; ++i)
				Assert::AreEqual(abs_result[i], result_function[i]);
		}
	};
}
