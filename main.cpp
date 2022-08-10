#include <assert.h>
#include <math.h>
#include <time.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <string>
#include <filesystem>

#include "CSSParser.h"
#include "CSSProperties.h"
#include "CSSUtils.h"
#include "cmdline.h"

std::vector<std::string> parse_arguments(int argc, char** argv)
{
	std::vector<std::string> files;

	cmdline::parser a;
	a.add<std::string>("dir", 'd', "format qss file in dierctory", false);
	a.add<std::string>("help", 'h', "print help message", false);

	a.parse_check(argc, argv);

	std::filesystem::path current_path = std::filesystem::current_path();

	if (a.exist("dir"))
	{
		std::string dir = a.get<std::string>("dir");

		if (!dir.empty())
		{
			std::filesystem::path dir_path(dir);
			if (!dir_path.is_absolute())
			{
				dir_path = std::filesystem::current_path() / dir_path;
			}

			if (std::filesystem::exists(dir_path))
			{
				for (auto& p : std::filesystem::directory_iterator(dir_path))
				{
					if (p.is_regular_file() && p.path().extension() == ".qss")
					{
						files.push_back(p.path().string());
					}
				}
			}
		}
	}

	for (int i = 0; i < a.rest().size(); i++)
	{
		std::string file = a.rest().at(i);

		if (!file.empty())
		{
			std::filesystem::path file_path = current_path / file;
			files.push_back(file_path.string());
		}
	}
	return files;
}

void format_file(std::string path, bool in_place)
{
	CSSParser csst;

	std::string css_file = CSSUtils::file_get_contents(path);
	csst.set_level("CSS3.0");
	csst.parse_css(css_file);

	// check for any parse errors
	std::vector<std::string> errors = csst.get_parse_errors();
	std::cout << "Errors: " << errors.size() << std::endl;
	for (int i = 0; i < errors.size(); i++)
	{
		std::cout << "  Error: " << errors[i] << std::endl;
	}

	// check for any parse warnings
	std::vector<std::string> warnings = csst.get_parse_warnings();
	std::cout << "Warnings: " << warnings.size() << std::endl;
	for (int i = 0; i < warnings.size(); i++)
	{
		std::cout << "  Warning: " << warnings[i] << std::endl;
	}

	std::vector<std::string> infos = csst.get_parse_info();
	std::cout << "Information: " << infos.size() << std::endl;
	for (int i = 0; i < infos.size(); i++)
	{
		std::cout << "  Information: " << infos[i] << std::endl;
	}

	std::string cssout;
	if (!in_place)
	{
		cssout = csst.serialize_css();
	}
	else
	{
		cssout = csst.serialize_css(path, false);
	}

	return;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> files = parse_arguments(argc, argv);

	for (auto file : files)
	{
		std::cout << file << std::endl;
		format_file(file, true);
		std::cout << std::endl << std::endl;
	}

	return 0;
}

