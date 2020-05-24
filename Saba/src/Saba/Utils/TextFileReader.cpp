#include "pch.h"
#include "TextFileReader.h"

namespace Saba {

	std::pair<std::string_view, bool> TextFileReader::GetLine(TextFile& file)
	{
		uint32_t a = file.m_Data.find('\n', (std::size_t)file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - file.m_AtID;
		if (a != file.m_Data.size()) file.m_AtID = a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextFileReader::GetLine(TextFile& file, uint32_t line)
	{
		uint32_t a = 0;
		for (uint32_t i = 0; i < line; i++)
		{
			a = file.m_Data.find('\n', (std::size_t)a) + 1;
			if (a == -1)
				return {};
		}
		uint32_t b = file.m_Data.find('\n', (std::size_t)a);
		if (b == -1) b = file.m_Data.size();
		return std::string_view(&file.m_Data[a], b - a);
	}
	std::pair<char, bool> TextFileReader::GetChar(TextFile& file)
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
	char TextFileReader::GetChar(TextFile& file, uint32_t pos)
	{
		if (pos >= file.m_Data.size()) return {};
		return file.m_Data[pos];
	}
	std::pair<std::string_view, bool> TextFileReader::GetTillChar(TextFile& file, char character)
	{
		uint32_t a = file.m_Data.find(character, file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - file.m_AtID;
		if (a != -1) file.m_AtID = a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextFileReader::GetTillChar(TextFile& file, char character, uint32_t offset)
	{
		if (offset >= file.m_Data.size()) return {};
		uint32_t a = file.m_Data.find(character, offset);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - offset;
		return std::string_view(&file.m_Data[offset], b);
	}
	std::pair<std::string_view, bool> TextFileReader::GetTillText(TextFile& file, const std::string& text)
	{
		uint32_t a = file.m_Data.find(text, file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - file.m_AtID;
		if (a != -1) file.m_AtID = a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextFileReader::GetTillText(TextFile& file, const std::string& text, uint32_t offset)
	{
		if (offset >= file.m_Data.size()) return {};
		uint32_t a = file.m_Data.find(text, offset);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - offset;
		return std::string_view(&file.m_Data[offset], b);
	}
	std::pair<std::string_view, bool> TextFileReader::GetTillOneOf(TextFile& file, const std::string& chars)
	{
		uint32_t a = file.m_Data.find_first_of(chars, file.m_AtID);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - file.m_AtID;
		if (a != -1) file.m_AtID = a + 1;
		else file.m_AtID = 0;
		std::string_view view(file.m_At, b);
		file.m_At = &file.m_Data[file.m_AtID];
		return { view, file.m_AtID == 0 ? true : false };
	}
	std::string_view TextFileReader::GetTillOneOf(TextFile& file, const std::string& chars, uint32_t offset)
	{
		if (offset >= file.m_Data.size()) return {};
		uint32_t a = file.m_Data.find_first_of(chars, offset);
		if (a == -1) a = file.m_Data.size();
		uint32_t b = a - offset;
		return std::string_view(&file.m_Data[offset], b);
	}
}
