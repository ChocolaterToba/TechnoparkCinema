#pragma once
#include <exception>

class ParsingException: public std::exception {};
class UnknownMethodException: public std::exception {};
class BadFormatException: public ParsingException {};


