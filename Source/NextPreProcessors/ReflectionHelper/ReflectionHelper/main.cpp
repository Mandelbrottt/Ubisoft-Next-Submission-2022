#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

static
void
FindAllInstancesOfReflectRegister(fs::path const& a_sourceRoot);

static
void
AddCompilerDirectivesToIncludeFile(fs::path const& a_location);

std::vector<std::string> g_reflectedTypeNames;

int
main()
{
	fs::path path = fs::current_path();

	while (!fs::exists(path / "Source"))
	{
		if (!path.has_parent_path())
		{
			exit(1);
		}

		path = path.parent_path();
	}

	auto sourceDir = path / "Source";

	FindAllInstancesOfReflectRegister(sourceDir);

	AddCompilerDirectivesToIncludeFile(sourceDir / "NextBootstrap" / "NextBootstrap");
}

void
FindAllInstancesOfReflectRegister(fs::path const& a_sourceRoot)
{
	fs::recursive_directory_iterator directory_entries { a_sourceRoot };

	for (auto const& entry : directory_entries)
	{
		std::error_code code;
		if (!entry.is_regular_file(code))
		{
			continue;
		}

		auto path = entry.path();

		if (!path.has_extension() || path.extension() != ".cpp")
		{
			continue;
		}

		if (equivalent(path, __FILE__))
		{
			continue;
		}

		std::ifstream ifs { path };

		const char*  macroName       = "ReflectRegister(";
		const size_t macroNameLength = strlen(macroName);

		std::string line;
		while (!ifs.eof())
		{
			std::getline(ifs, line);

			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

			std::string::size_type beginIndex = 0;

			while ((beginIndex = line.find(macroName, beginIndex)) != std::string::npos)
			{
				beginIndex += macroNameLength;

				auto endIndex = line.find(')', beginIndex);

				auto typeName = line.substr(beginIndex, endIndex - beginIndex);
				
				g_reflectedTypeNames.push_back(typeName);

				printf("Found auto-registered reflected type \"%s\"\n", typeName.c_str());
			}
		}
	}
}

static
void
AddCompilerDirectivesToIncludeFile(fs::path const& a_location)
{
	std::ofstream stream { a_location / "Include.gen.cpp" };

	for (auto const& typeName : g_reflectedTypeNames)
	{
		auto newTypeName = "_REFLECT_REGISTER_" + typeName;
		stream << "#pragma comment(linker, \"/include:" << newTypeName << "\")\n";
	}
}
