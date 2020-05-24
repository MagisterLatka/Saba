#pragma once
#include "TextFile.h"

namespace Saba {

	class TextFileReader
	{
	public:
		static std::pair<std::string_view, bool> GetLine(TextFile& file);
		static std::string_view GetLine(TextFile& file, uint32_t line);
		static std::pair<char, bool> GetChar(TextFile& file);
		static char GetChar(TextFile& file, uint32_t pos);
		static std::pair<std::string_view, bool> GetTillChar(TextFile& file, char character);
		static std::string_view GetTillChar(TextFile& file, char character, uint32_t offset);
		static std::pair<std::string_view, bool> GetTillText(TextFile& file, const std::string& text);
		static std::string_view GetTillText(TextFile& file, const std::string& text, uint32_t offset);
		static std::pair<std::string_view, bool> GetTillOneOf(TextFile& file, const std::string& chars);
		static std::string_view GetTillOneOf(TextFile& file, const std::string& chars, uint32_t offset);
	};
}
