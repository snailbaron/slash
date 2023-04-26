#include <arg.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    auto inputDir = arg::option<fs::path>()
        .markRequired()
        .keys("-i", "--input")
        .help("path to directory with resource files");
    auto outputSheet = arg::option<fs::path>()
        .markRequired()
        .keys("--sheet")
        .help("path to output sprite sheet");
    auto outputSheetJson = arg::option<fs::path>()
        .markRequired()
        .keys("--sheet-json")
        .help("path to output sprite sheet data in JSON format");
    arg::helpKeys("-h", "--help");
    arg::parse(argc, argv);

    auto asepriteFilePaths = std::vector<fs::path>{};
    for (const auto& entry : fs::recursive_directory_iterator{inputDir}) {
        if (entry.path().extension() == ".aseprite") {
            std::cout << "packing " << entry.path() << "\n";
            asepriteFilePaths.push_back(entry.path());
        }
    }

    auto cmd = std::ostringstream{};
    cmd << "aseprite --batch";
    for (const auto& path : asepriteFilePaths) {
        cmd << " '" << path.string() << "'";
    }
    cmd << " --sheet '" << outputSheet->string() << "'" <<
        " --sheet-type packed" <<
        " --data '" << outputSheetJson->string() << "'" <<
        " --format json-array" <<
        " --filename-format '{title}-{tag}-{tagframe}'";
    std::cout << "packing aseprite resources: " << cmd.str() << "\n";
    std::system(cmd.str().c_str());
}
