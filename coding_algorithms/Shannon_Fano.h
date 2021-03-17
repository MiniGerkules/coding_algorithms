#pragma once

#include "RB_tree/Map.h"
#include "array_elem.h"
#include <string>


// Shannon-Fano coding class
class Shannon_Fano
{
private:
	Map<char, size_t>* counting_tree;				// Red-black tree, in which we will count the number in search of
	Map<char, std::string>* code_tree;				// Red-black tree in which we will store frequency and codes
	const std::string uncoded;						// The string to encode
	size_t number_def_letter;						// Number of different letters

	// Red-black tree filling method
	void filling();

	// The method building Shannon-Fano codes
	void coding() const;

	/*
		A method that divides symbols by frequency of occurrence and writes codes into a tree
		Array_elem<char, size_t>* elems -- sorted frequency table
		size_t i_front -- index from the beginning of the array
		size_t i_back -- index from the end of the array
		std::string code -- current code
		size_t counter -- counter to free memory
	*/
	void divider(const Array_elem<char, size_t>* elems, const size_t i_front, const size_t i_back, const std::string& code, size_t counter) const;

public:

	/*
		Shannon_Fano class constructor
		std::string input -- user input
	*/
	Shannon_Fano(std::string& input);

	// Shannon_Fano class destructor
	~Shannon_Fano();

	// Method that returns field number_def_letter
	size_t get_number_def_letter() const { return number_def_letter; }

	// The method returns the resulting codes of all characters
	std::string* get_all_codes() const;

	/*
		Method that returns the code of the transferred character
		char c -- The character to be returned
	*/
	std::string get_the_code(char c) const;

	// The method that encodes the string passed to the constructor
	std::string string_encoding() const;

	/*
		Method to decode the passed string
		std::string& coded -- encoded string
	*/
	std::string string_decoding(std::string& coded) const;

	// The method calculates the volume of the original and encoded strings, as well as the compression ratio
	size_t* encoding_result() const;

	// The method prints characters, their frequency and code
	void print_all() const;
};
