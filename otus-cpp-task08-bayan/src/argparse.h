#pragma once
#include <boost/program_options.hpp>
#include <config.h>

TConfig parseConfig(int argc, char* argv[]) {
    namespace po = boost::program_options;
    using namespace std;

    TConfig sConfig;
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("include,i",       po::value(&sConfig.IncludeToSearchDirs), "directories to search, multiple ('.' by default)")
            ("exclude,e",       po::value(&sConfig.ExcludeToSearchDirs), "directories to exclude, multiple")
            ("mask",            po::value(&sConfig.FilesMask), "file mask to check, multiple.")
            ("min-file-size,m", po::value(&sConfig.MinCheckFileSizeBytes)->default_value(1), "min file size to check")
            (",r",              po::bool_switch(&sConfig.Recursive)->default_value(false), "recursive")
            ("block-size,b",    po::value(&sConfig.CheckBlockSize)->default_value(100), "block size to check by algo")
            ("method",          po::value(&sConfig.CheckMethod)->default_value(TConfig::ECheckMethod::CRC32), "check algorithm (crc32/md5)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || vm.count("h")) {
        cout << desc << "\n";
        exit(0);
    }
    if(sConfig.IncludeToSearchDirs.empty()) {
        sConfig.IncludeToSearchDirs.emplace_back(".");
    }

//    std::cout << sConfig << "\n" << std::endl;

    if (!sConfig.IsValid()) {
        throw std::runtime_error("Invalid arguments set");
    }
    return sConfig;
}
