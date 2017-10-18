#pragma once
class InputFile
{
public:
	InputFile(const fs::path file_path);
	InputFile(const InputFile &) = delete;
	virtual ~InputFile();
	InputFile & operator=(const InputFile &) = delete;

	void MoveToEndOfFile();

	bool is_open() const;
	bool eof() const;
	bool getline(std::string & line);

private:
	const fs::path m_file_path;
	std::wifstream m_ifstream;
	std::streampos m_file_position;
	bool m_debug_logging_enabled;
	std::string m_wide_string_to_byte_string(const std::wstring & wstr);
	bool m_has_bom();
	bool m_open_file();
};

