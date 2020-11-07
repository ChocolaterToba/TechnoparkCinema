#pragma once

#include <exception>

class ParsingException: public std::exception {};
class UnknownMethodException: public ParsingException {};
class BadFormatException: public ParsingException {};

class HTTPResponseException: public std::exception {};
class OldVersionException: public HTTPResponseException {};
class FileNotFoundException: public HTTPResponseException {};

