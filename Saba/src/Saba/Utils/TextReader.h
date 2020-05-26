#pragma once
#include "TextFile.h"

namespace Saba {

	class TextReader
	{
	public:
		static std::pair<std::string_view, bool> GetLine(TextFile& file);
		static std::string_view GetLine(TextFile& file, size_t line);
		static std::pair<char, bool> GetChar(TextFile& file);
		static char GetChar(TextFile& file, size_t pos);
		static std::pair<std::string_view, bool> GetTillChar(TextFile& file, char character);
		static std::string_view GetTillChar(TextFile& file, char character, size_t offset);
		static std::pair<std::string_view, bool> GetTillText(TextFile& file, const std::string& text);
		static std::string_view GetTillText(TextFile& file, const std::string& text, size_t offset);
		static std::pair<std::string_view, bool> GetTillOneOf(TextFile& file, const std::string& chars);
		static std::string_view GetTillOneOf(TextFile& file, const std::string& chars, size_t offset);

		static std::optional<std::string_view> GetTillChar(std::string_view source, char character, size_t offset = 0);
		static std::optional<std::string_view> GetTillText(std::string_view source, const std::string& text, size_t offset = 0);
		static std::optional<std::string_view> GetTillOneOf(std::string_view source, const std::string& chars, size_t offset = 0);
	};
}
