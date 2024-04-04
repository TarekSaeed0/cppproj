#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <array>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		throw std::invalid_argument("expected a single argument of a project path");
	}
	std::filesystem::path project_path(argv[1]);
	std::filesystem::create_directory(project_path);

	std::filesystem::path project_name = project_path.filename();

	std::unique_ptr<FILE, decltype(&pclose)> cmake_version_command_file(popen("cmake --version", "r"), pclose);
	if(!cmake_version_command_file) {
		throw std::runtime_error("failed to get cmake version");
	}
	std::array<char, 64> cmake_version_command_buffer;
	std::fgets(cmake_version_command_buffer.data(), static_cast<int>(cmake_version_command_buffer.size()), cmake_version_command_file.get());
	std::string cmake_version = std::string(cmake_version_command_buffer.data() + std::strlen("cmake version "));
	cmake_version.erase(cmake_version.find_last_not_of("\n") + 1);

	std::filesystem::path project_cmakelists_txt_path = project_path / "CMakeLists.txt";
	std::ofstream project_cmakelists_txt_file(project_cmakelists_txt_path);
	if (!project_cmakelists_txt_file.is_open()) {
		throw std::runtime_error("failed to create file CMakeLists.txt");
	}
	project_cmakelists_txt_file <<
		"cmake_minimum_required(VERSION " << cmake_version << ")\n"
		"project(" << project_name.string() << ")\n"
		"\n"
		"set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n"
		"set(CMAKE_BUILD_TYPE Debug)\n"
		"\n"
		"add_executable(${PROJECT_NAME} source/main.cpp)\n"
		"target_include_directories(${PROJECT_NAME} PRIVATE header)\n"
		"target_link_directories(${PROJECT_NAME} PRIVATE library)\n"
		"target_compile_options(${PROJECT_NAME} PRIVATE\n"
		"    -std=c++20 -O2 -Werror -Wall -Wextra -pedantic -Wfloat-equal -Wundef\n"
		"    -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5\n"
		"    -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum\n"
		"    -Wconversion -Wunreachable-code\n"
		")\n";
	project_cmakelists_txt_file.close();

	std::filesystem::path project_compile_commands_json_path = project_path / "compile_commands.json";
	std::filesystem::create_symlink("build/compile_commands.json", project_compile_commands_json_path);

	std::filesystem::path project_source_path = project_path / "source";
	std::filesystem::create_directory(project_source_path);

	std::filesystem::path project_source_main_cpp_path = project_source_path / "main.cpp";
	std::ofstream project_source_main_cpp_file(project_source_main_cpp_path);
	if (!project_source_main_cpp_file.is_open()) {
		throw std::runtime_error("failed to create file source/main.cpp");
	}
	project_source_main_cpp_file <<
		"#include <iostream>\n"
		"\n"
		"int main() {\n"
		"	std::cout << \"Hello, World!\\n\";\n"
		"}\n";
	project_source_main_cpp_file.close();

	std::filesystem::path project_resource_path = project_path / "resource";
	std::filesystem::create_directory(project_resource_path);

	std::filesystem::path project_header_path = project_path / "header";
	std::filesystem::create_directory(project_header_path);

	std::filesystem::path project_library_path = project_path / "library";
	std::filesystem::create_directory(project_library_path);

	std::filesystem::path project_build_path = project_path / "build";

	std::stringstream cmake_build_command;
	cmake_build_command << "cmake -S " << project_path << " -B " << project_build_path << " > /dev/null";
	if (std::system(cmake_build_command.str().c_str()) != 0) {
		throw std::runtime_error("failed to generate build system");
	}
}
