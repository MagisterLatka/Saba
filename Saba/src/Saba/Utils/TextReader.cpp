#include "pch.h"
#include "TextReader.h"

namespace Saba {

	std::pair<std::string_view, bool> TextReader::GetLine(TextFile& file)
	{
		size_t a = file.m_Data.find('\n', (size_t)file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - (size_t)file.m_AtID;
		if (a != file.m_Data.size()) file.m_AtID = (uint32_t)a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextReader::GetLine(TextFile& file, size_t line)
	{
		size_t a = 0;
		for (uint32_t i = 0; i < line; i++)
		{
			a = file.m_Data.find('\n', a) + 1;
			if (a == -1)
				return {};
		}
		size_t b = file.m_Data.find_first_of("\r\n", a);
		if (b == -1) b = file.m_Data.size();
		return std::string_view(&file.m_Data[a], b - a);
	}
	std::pair<char, bool> TextReader::GetChar(TextFile& file)
	{
		char a = *file.m_At;
		file.m_At++;
		file.m_AtID++;
		if (file.m_AtID == file.m_Data.size())
		{
			file.m_At = &file.m_Data[0];
			file.m_AtID = 0;
		}
		return { a, file.m_AtID == 0 ? true : false };
	}
	char TextReader::GetChar(TextFile& file, size_t pos)
	{
		if (pos >= file.m_Data.size()) return {};
		return file.m_Data[pos];
	}
	std::pair<std::string_view, bool> TextReader::GetTillChar(TextFile& file, char character)
	{
		size_t a = file.m_Data.find(character, file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - file.m_AtID;
		if (a != -1) file.m_AtID = (uint32_t)a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextReader::GetTillChar(TextFile& file, char character, size_t offset)
	{
		if (offset >= file.m_Data.size()) return {};
		size_t a = file.m_Data.find(character, offset);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - offset;
		return std::string_view(&file.m_Data[offset], b);
	}
	std::pair<std::string_view, bool> TextReader::GetTillText(TextFile& file, const std::string& text)
	{
		size_t a = file.m_Data.find(text, file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - file.m_AtID;
		if (a != -1) file.m_AtID = (uint32_t)a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextReader::GetTillText(TextFile& file, const std::string& text, size_t offset)
	{
		if (offset >= file.m_Data.size()) return {};
		size_t a = file.m_Data.find(text, offset);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - offset;
		return std::string_view(&file.m_Data[offset], b);
	}
	std::pair<std::string_view, bool> TextReader::GetTillOneOf(TextFile& file, const std::string& chars)
	{
		size_t a = file.m_Data.find_first_of(chars, file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - file.m_AtID;
		if (a != -1) file.m_AtID = (uint32_t)a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextReader::GetTillOneOf(TextFile& file, const std::string& chars, size_t offset)
	{
		if (offset >= file.m_Data.size()) return {};
		size_t a = file.m_Data.find_first_of(chars, offset);
		if (a == -1) a = file.m_Data.size();
		size_t b = a - offset;
		return std::string_view(&file.m_Data[offset], b);
	}

	std::optional<std::string_view> TextReader::GetTillChar(std::string_view source, char character, size_t offset)
	{
		if (offset >= source.size()) return {};
		size_t a = source.find(character, offset);
		if (a != -1)
			return std::string_view(&source[offset], a - offset);
		else
			return std::string_view(&source[offset], source.size() - offset);
	}
	std::optional<std::string_view> TextReader::GetTillText(std::string_view source, const std::string& text, size_t offset)
	{
		if (offset >= source.size()) return {};
		size_t a = source.find(text, offset);
		if (a != -1)
			return std::string_view(&source[offset], a - offset);
		else
			return std::string_view(&source[offset], source.size() - offset);
	}
	std::optional<std::string_view> TextReader::GetTillOneOf(std::string_view source, const std::string& chars, size_t offset)
	{
		if (offset >= source.size()) return {};
		size_t a = source.find_first_of(chars, offset);
		if (a != -1)
			return std::string_view(&source[offset], a - offset);
		else
			return std::string_view(&source[offset], source.size() - offset);
	}
}
