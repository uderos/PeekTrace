#pragma once

#define g_CONFIG (Config::Instance())

class Config
{
	public:

		static Config & Instance();

		void ProcessCmdLine(const int argc, const char *argv[]);
		void Dump() const;

		const fs::path & GetInpoutFilePath() const;
		bool GetVerboseFlag() const;
		bool GetDebugFlag() const;
		bool GetTailFlag() const;
		const std::vector<std::string> & GetANDFilters() const;
		const std::vector<std::string> & GetORFilters() const;

	private:

		static Config * m_instance_ptr;

		bool m_has_configuration;
		bool m_verbose_flag;
		bool m_debug_flag;
		bool m_all_file_flag;
		std::vector<std::string> m_and_filters;
		std::vector<std::string> m_or_filters;

		fs::path m_input_file_path;

		Config();
		virtual ~Config();

		std::string m_get_prog_name_and_version() const;
};
