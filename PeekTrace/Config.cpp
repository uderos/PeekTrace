#include "stdafx.h"
#include "Config.h"
#include "exh.h"


namespace po = boost::program_options;

static const std::string f_PROGRAM_NAME("PeekTrace");
static const std::string f_PROGRAM_VERSION("0.1");

static const std::string DEFAULT_INPUT_FILE{
	(R"(C:\ws\ws_supertop_01\TOP\CM\SW\backup\CurrentTraceFile.txt)") 
//	(R"(C:\Program Files (x86)\IL\ACL-TOP\backup\CurrentTraceFile.txt)")
};

Config * Config::m_instance_ptr = nullptr;

Config::Config() :
	m_has_configuration(false),
	m_verbose_flag(false),
	m_debug_flag(false),
	m_all_file_flag(false)
{
}


Config::~Config()
{
}

Config & Config::Instance()
{
	if (!m_instance_ptr)
		m_instance_ptr = new Config;
	return (*m_instance_ptr);
}

void Config::ProcessCmdLine(const int argc, const char *argv[])
{
	const std::string desc_str = m_get_prog_name_and_version() + std::string(" - Allowed options");
	po::options_description desc(desc_str);

	desc.add_options()
		("help,h", "Produce this help message")
		("verbose,v", "Verbose logging on stdout")
		("debug,d", "Enable debugging features")
		("file,f", po::value<std::string>()->default_value(DEFAULT_INPUT_FILE), "Input file")
		("all-file,A", "Process the entire (pre-existing) file")
		("and,a", po::value<std::vector<std::string>>(&m_and_filters), "AND filter")
		("or,o", po::value<std::vector<std::string>>(&m_or_filters), "OR filter")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
	}
	else
	{
		po::notify(vm);

		m_verbose_flag = (vm.count("verbose") > 0);
		m_debug_flag = (vm.count("debug") > 0);
		m_input_file_path = fs::path(vm["file"].as<std::string>());
		m_all_file_flag = (vm.count("all-file") > 0);

		if (m_verbose_flag)
			Dump();
	}

	m_has_configuration = true;
}

void Config::Dump() const
{
	std::cout << "Configuration:"
		<< "\n\t input-file=" << m_input_file_path
		<< "\n\t verbose=" << m_verbose_flag
		<< "\n\t all-file=" << m_all_file_flag;

	for (const auto s : m_and_filters)
		std::cout << "\n\t AND " << s;

	for (const auto s : m_or_filters)
		std::cout << "\n\t OR " << s;

	std::cout << std::endl;

}

const fs::path & Config::GetInpoutFilePath() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_input_file_path;
}

bool Config::GetVerboseFlag() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_verbose_flag;
}

bool Config::GetDebugFlag() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_debug_flag;
}

bool Config::GetTailFlag() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return (!m_all_file_flag);
}

void Config::GetFilters(
	std::vector<std::string> & and_filters,
	std::vector<std::string> & or_filters)
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	and_filters = m_and_filters;
	or_filters = m_or_filters;
}

std::string Config::m_get_prog_name_and_version() const
{
	std::ostringstream oss;
	oss << f_PROGRAM_NAME << " v" << f_PROGRAM_VERSION;
	return oss.str();
}