#pragma once

namespace Saba {

	class TextFile
	{
		friend class TextFileReader;
	public:
		enum Mode {
			None = 0, Input = BIT(0), Output = BIT(1), Binary = BIT(2), Append = BIT(3), AtEnd = BIT(4), Trunc = BIT(5)
		};
	public:
		TextFile(const std::string& filepath, Mode mode);
		TextFile(const std::string& filepath, int mode);
		TextFile(const TextFile& other);
		TextFile& operator=(const TextFile& other);
		TextFile(TextFile&& other) noexcept;
		TextFile& operator=(TextFile&& other) noexcept;
		~TextFile();

		void Reopen(const std::string& filepath, Mode mode);

		inline const std::string& Get() const { return m_Data; }
		inline Mode GetMode() const { return m_Mode; }
	private:
		static std::ios::openmode GetSTDOpenMode(Mode mode);
	private:
		std::string m_Data;
		char* m_At;
		uint32_t m_AtID = 0;
		Mode m_Mode;
	};
}
