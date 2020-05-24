#include "pch.h"
#include "TextFile.h"

#include <filesystem>

namespace Saba {

	TextFile::TextFile(const std::string& filepath, Mode mode)
		: m_Mode(mode)
	{
		std::fstream stream(filepath, GetSTDOpenMode(mode));
		uintmax_t fileSize = std::filesystem::file_size(filepath);
		m_Data = std::string(fileSize + 1, 0);
		stream.read(&m_Data[0], fileSize);
		m_At = &m_Data[0];
	}
	TextFile::TextFile(const std::string& filepath, int mode)
		: m_Mode((Mode)mode)
	{
		std::fstream stream(filepath, GetSTDOpenMode(m_Mode));
		uintmax_t fileSize = std::filesystem::file_size(filepath);
		m_Data = std::string(fileSize + 1, 0);
		stream.read(&m_Data[0], fileSize);
		m_At = &m_Data[0];
	}
	TextFile::TextFile(const TextFile& other)
		: m_Data(other.m_Data), m_Mode(other.m_Mode), m_At(&m_Data[0])
	{
	}
	TextFile& TextFile::operator=(const TextFile& other)
	{
		m_Data = other.m_Data;
		m_Mode = other.m_Mode;
		return *this;
	}
	TextFile::TextFile(TextFile&& other) noexcept
		: m_Data(std::move(other.m_Data)), m_Mode(std::exchange(other.m_Mode, Mode::None)), m_At(&m_Data[0])
	{
	}
	TextFile& TextFile::operator=(TextFile&& other) noexcept
	{
		m_Data = std::move(other.m_Data);
		m_Mode = std::exchange(other.m_Mode, Mode::None);
		return *this;
	}
	TextFile::~TextFile()
	{
	}

	void TextFile::Reopen(const std::string& filepath, Mode mode)
	{
		std::fstream stream(filepath, GetSTDOpenMode(mode));
		uintmax_t fileSize = std::filesystem::file_size(filepath);
		m_Data = std::string(fileSize + 1, 0);
		stream.read(&m_Data[0], fileSize);
		m_At = &m_Data[0];
	}

	std::ios::openmode TextFile::GetSTDOpenMode(Mode mode)
	{
		std::ios::openmode m = 0;
		if (mode & Mode::Input)
			m |= std::ios::in;
		if (mode & Mode::Output)
			m |= std::ios::out;
		if (mode & Mode::Binary)
			m |= std::ios::binary;
		if (mode & Mode::Append)
			m |= std::ios::app;
		if (mode & Mode::AtEnd)
			m |= std::ios::ate;
		if (mode & Mode::Trunc)
			m |= std::ios::trunc;
		return m;
	}
}
