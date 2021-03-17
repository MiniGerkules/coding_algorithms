#include "Shannon_Fano.h"
#include <stdexcept>
#include <cmath>

//----------------------------------------------
// Private methods of class Shannon_Fano
//----------------------------------------------

// Red-black tree filling method
void Shannon_Fano::filling()
{
	Node<char, size_t>* current;					// Current tree node
	size_t counter = 0;								// Number of different letters

	for (char c : uncoded)
	{
		try
		{
			current = counting_tree->find(c);
			current->set_value(current->get_value() + 1);
		}
		catch (domain_error)
		{
			counting_tree->insert(c, 1);
			++counter;
		}
	}

	number_def_letter = counter;
	coding();
}

// The method building Shannon-Fano codes
void Shannon_Fano::coding() const
{
	Array_elem<char, size_t>* elems;

	elems = new Array_elem<char, size_t>[number_def_letter];
	elems = counting_tree->take_elems(elems, number_def_letter);

	sort(elems, elems + number_def_letter,
		[](Array_elem<char, size_t>& a, Array_elem<char, size_t>& b) { return a.value < b.value; });

	divider(elems, 0, number_def_letter - 1, "", 0);
}

/*
	A method that divides symbols by frequency of occurrence and writes codes into a tree
	Array_elem<char, size_t>* elems -- sorted frequency table
	size_t i_front -- index from the beginning of the array
	size_t i_back -- index from the end of the array
	std::string code -- current code
	size_t counter -- counter to free memory
*/
void Shannon_Fano::divider(const Array_elem<char, size_t>* elems, const size_t i_front, const size_t i_back, 
								const std::string& code, size_t counter) const
{
	if ((i_front + 1 != i_back) && (i_front != i_back))
	{
		size_t counter_front;							// Sum of elements from the beginning of the array
		size_t counter_back;							// Sum of elements from the end of the array
		size_t front = i_front;							// Current index from the beginning of the array
		size_t back = i_back;							// The current index from the end of the array

		counter_front = elems[front].value;
		counter_back = elems[back].value;

		while (true)
		{
			if (front + 1 == back)
				break;
			else if (counter_front < counter_back)
				counter_front += elems[++front].value;
			else
				counter_back += elems[--back].value;
		}

		// Recursively call in both parts
		divider(elems, i_front, front, code + "0", counter);
		divider(elems, back, i_back, code + "1", counter);
	}
	else if (i_front + 1 == i_back)
	{
		code_tree->insert(elems[i_front].key, code + "0");
		code_tree->insert(elems[i_back].key, code + "1");
		counter += 2;
	}
	else
	{
		code_tree->insert(elems[i_front].key, code);
		++counter;
	}

	// Freeing up memory
	if (counter == number_def_letter)
		delete[] elems;
}

//----------------------------------------------
// Public methods of class Shannon_Fano
//----------------------------------------------

/*
	Shannon_Fano class constructor
	std::string input -- user input
*/
Shannon_Fano::Shannon_Fano(std::string& input) 
	: uncoded(std::move(input)), number_def_letter(0)
{
	counting_tree = new Map<char, size_t>();
	code_tree = new Map<char, std::string>();

	// Filling in the red-black tree
	filling();
}

// Shannon_Fano class destructor
Shannon_Fano::~Shannon_Fano()
{
	delete counting_tree;
	delete code_tree;
}

// The method returns the resulting codes of all characters
std::string* Shannon_Fano::get_all_codes() const
{
	Iterator<char, std::string>* bft;				// BFT iterator
	Node<char, std::string>* current;				// Current node
	std::string* result = nullptr;					// All codes
	size_t i = 0;									// The index of the added item

	bft = code_tree->create_bft_iterator();
	result = new std::string[number_def_letter];

	while (bft->has_next())
	{
		current = bft->next();
		result[i++] = current->get_value();
	}

	return result;
}

/*
	Method that returns the code of the transferred character
	char c -- The character to be returned
*/
std::string Shannon_Fano::get_the_code(char c) const
{
	std::string result;								// Found code

	try
	{
		result = code_tree->find(c)->get_value();
	}
	catch (std::domain_error error)
	{
		std::cout << error.what() << std::endl;
		result = "";
	}

	return result;
}

// The method that encodes the string passed to the constructor
std::string Shannon_Fano::string_encoding() const
{
	std::string encoded = "";						// Encoding result

	for (char c : uncoded)
		encoded += code_tree->find(c)->get_value();

	return encoded;
}

/*
	Method to decode the passed string
	std::string& coded -- encoded string
*/
std::string Shannon_Fano::string_decoding(std::string& coded) const
{
	Array_elem<char, std::string>* elems;
	std::string decoded = "";						// Decoded string

	elems = new Array_elem<char, std::string>[number_def_letter];
	elems = code_tree->take_elems(elems, number_def_letter);

	for (size_t i = 0; i < coded.length(); ++i)
		for (size_t j = 0; j < number_def_letter; ++j)
			if (coded.length() - i >= elems[j].value.length())
			{
				size_t bias = 0;					// Main line offset
				bool flag = true;					// Flag indicating whether the current character is suitable for decoding

				for (size_t k = 0; k < elems[j].value.length(); ++k, ++bias)
					if (coded[i + bias] != elems[j].value[k])
					{
						flag = false;
						break;
					}

				if (flag)
				{
					i += bias - 1;
					decoded += elems[j].key;
					break;
				}
			}

	delete[] elems;

	return decoded;
}

// The method calculates the volume of the original and encoded strings, as well as the compression ratio
size_t* Shannon_Fano::encoding_result() const
{
	size_t* result = new size_t[3] { 0, 0, 0 };		// Line sizes and compression ratio
	size_t uniform_coding = 1;						// Uniform coding bits

	while (pow(2, uniform_coding) < number_def_letter)
		++uniform_coding;

	result[0] = uncoded.length() * uniform_coding;
	for (size_t i = 0; i < uncoded.length(); ++i)
		result[1] += code_tree->find(uncoded[i])->get_value().length();

	result[2] = round(100 * (1 - (float)result[1] / result[0]));

	return result;
}

// The method prints characters, their frequency and code
void Shannon_Fano::print_all() const
{
	Iterator<char, std::string>* bft = code_tree->create_bft_iterator();
	Node<char, std::string>* current;
	std::string line = string_encoding();

	std::cout << "The encoded source string -- " << line << std::endl;
	std::cout << "Decoded source string -- " << string_decoding(line) << std::endl;

	std::cout << std::endl;
	while (bft->has_next())
	{
		current = bft->next();
		std::cout << "The character " << current->get_key() << " is coded " << current->get_value() << std::endl;
	}

	size_t* result = encoding_result();
	std::cout << "\nOriginal line size -- " << result[0] << " bits" << std::endl;
	std::cout << "Compressed line size -- " << result[1] << " bits" << std::endl;
	std::cout << "Compression ratio -- " << result[2] << "%" << std::endl;

	delete bft;
}
