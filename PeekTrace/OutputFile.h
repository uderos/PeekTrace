#pragma once
class OutputFile
{
public:
	OutputFile(std::ostream & os);
	virtual ~OutputFile();

	void write_string(const std::string & str);

private:
	std::ostream & m_ostream;
};

