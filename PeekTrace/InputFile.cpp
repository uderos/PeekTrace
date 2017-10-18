#include "stdafx.h"
#include "Config.h"
#include "exh.h"
#include "InputFile.h"


InputFile::InputFile(const fs::path file_path) :
	m_file_path(file_path),
	m_file_position(0),
	m_debug_logging_enabled(false)
{
	m_debug_logging_enabled = g_CONFIG.GetDebugFlag() && g_CONFIG.GetVerboseFlag();
	m_open_file();
}


InputFile::~InputFile()
{
}

void InputFile::MoveToEndOfFile()
{
	if (m_ifstream.is_open())
	{
		m_ifstream.clear(std::ios::eofbit);
		m_ifstream.seekg(0, m_ifstream.end);
		m_file_position = m_ifstream.tellg();
	}
}


bool InputFile::is_open() const
{
	return m_ifstream.good() && m_ifstream.is_open();
}

bool InputFile::eof() const
{
	if (m_ifstream.is_open()) THROW_RUNTIME_ERROR;
	return m_ifstream.eof();
}

bool InputFile::getline(std::string & line)
{

	if (!m_ifstream.is_open()) THROW_RUNTIME_ERROR;

	const bool debug_eof_bit = m_ifstream.eof();
	const bool debug_fail_bit = m_ifstream.fail();
	const bool debug_bad_bit = m_ifstream.bad();

	if (m_ifstream.eof())	m_ifstream.clear(std::ios::eofbit);
	if (m_ifstream.fail()) m_ifstream.clear(std::ios::failbit);
	if (m_ifstream.bad())	m_ifstream.clear(std::ios::badbit);

	m_ifstream.seekg(m_file_position);

	std::wstring wstr;
	bool has_line = std::getline(m_ifstream, wstr).good();

	if (has_line)
	{
		m_file_position = m_ifstream.tellg();
		line = m_wide_string_to_byte_string(wstr);
	}
	else if (m_debug_logging_enabled)
	{
		std::cout << "Nothing to read" 
			<< " pos=" << m_file_position
			<< " bad=" << debug_bad_bit << "-" << m_ifstream.bad()
			<< " fail=" << debug_fail_bit << "-" << m_ifstream.fail()
			<< " eof=" << debug_eof_bit << "-" << m_ifstream.eof()
			<< std::endl;
	}

	return has_line;
}


std::string InputFile::m_wide_string_to_byte_string(const std::wstring & wstr)
{
	constexpr std::wstring::value_type MAX_WCHAR{ 255 };
	constexpr char BAD_CHAR{ '.' };

	std::string bstr;
	bstr.reserve(wstr.length());

	for (const auto wc : wstr)
	{
		char bc = static_cast<char>(wc & 0xFF);

		if ((wc > MAX_WCHAR) || (!std::isprint(bc)))
			bc = BAD_CHAR;

		bstr.push_back(bc);
	}

	return bstr;	
}

bool InputFile::m_has_bom()
{
	bool has_bom = false;

	std::ifstream ifs(m_file_path.string(), std::ios::binary);

	if (ifs)
	{
		uint8_t byte1, byte2;
		ifs >> byte1, byte2;

		has_bom = (!ifs.eof()) && (byte1 == uint8_t(0xFF));
	}

	return has_bom;
}

bool InputFile::m_open_file()
{
	if (m_ifstream.is_open()) THROW_RUNTIME_ERROR;

	if (!fs::exists(m_file_path))
	{
		std::cout << "Input file does not exist: " << m_file_path.string() << std::endl;
		return false;
	}

	const bool has_bom = m_has_bom();

	m_ifstream.open(m_file_path.string(), std::ios::binary);
	if (!m_ifstream)
		return false;

	if (has_bom)
	{
		constexpr std::codecvt_mode mode =
			static_cast<std::codecvt_mode>(std::consume_header + std::little_endian);

		const std::locale my_locale(
			m_ifstream.getloc(),
			new std::codecvt_utf16<wchar_t, 0x10ffff, mode>);

		m_ifstream.imbue(my_locale);
	}
	else
	{
		constexpr std::codecvt_mode mode =
			static_cast<std::codecvt_mode>(std::little_endian);

		const std::locale my_locale(
			m_ifstream.getloc(),
			new std::codecvt_utf16<wchar_t, 0x10ffff, mode>);

		m_ifstream.imbue(my_locale);
	}

	return this->is_open();
}

//void InputFile::m_remove_eol(std::string & str) const
//{
//	static constexpr char LF_CHAR{ 0x0A };
//	static constexpr char CR_CHAR{ 0x0D };
//	static constexpr char EOL_CHAR{ '\n' };
//
//	if (!str.empty())
//	{
//		const std::size_t str_len = str.length();
//		std::size_t num_eol = 0;
//		for (std::size_t i = 0; i < str_len; ++i)
//		{
//			const std::size_t idx = str_len - i;
//			const char c = str[idx];
//			if ((c == LF_CHAR) || (c == CR_CHAR) || (c == EOL_CHAR))
//				++num_eol;
//			else
//				break;
//		}
//
//		if (num_eol)
//			str.erase(str_len - num_eol, std::string::npos);
//	}
//}
