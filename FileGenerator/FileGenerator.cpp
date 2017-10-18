// FileGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static const std::string f_FILE_ABS_NAME("C:\\temp\\testfile.txt");

int main()
{
	static constexpr auto SLEEP_TIME = std::chrono::milliseconds(750);
	static constexpr wchar_t EOL = 0x000A;
	

	for (std::size_t counter = 0; true; ++counter)
	{
		{
			const auto flags = (counter == 0 ? 
				std::ios::binary : 
				std::ios::app | std::ios::binary);

			std::wofstream ofs(f_FILE_ABS_NAME, flags);

			constexpr std::codecvt_mode mode =
				static_cast<std::codecvt_mode>(std::little_endian);

			const std::locale my_locale(
				ofs.getloc(),
				new std::codecvt_utf16<wchar_t, 0x10ffff, mode>);

			ofs.imbue(my_locale);


			ofs << L"Line_" << counter 
				<< ((counter % 2) ? L" odd" : L" even")
				<< EOL;
		}
		std::this_thread::sleep_for(SLEEP_TIME);
	}


    return 0;
}

