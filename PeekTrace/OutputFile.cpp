#include "stdafx.h"
#include "exh.h"
#include "OutputFile.h"


OutputFile::OutputFile(std::ostream & os) :
	m_ostream(os)
{
}


OutputFile::~OutputFile()
{
}

void OutputFile::write_string(const std::string & str)
{
	m_ostream << str << std::endl;
	if (!m_ostream.good()) THROW_RUNTIME_ERROR_ARG(std::string("Output Write Failure"));
}
