#include "fmt/core.h"
#include "gol_functions.hpp"
#include "gol_concepts.hpp"
#include "MapGOL.hpp"
#include "TMapGOL.hpp"
#include "fmt/ranges.h"
#include "cxxopts.hpp"
#include <string_view>
#include <vector>
#include <functional>


auto main(int argc, char** argv) -> int {
    auto options = cxxopts::Options("gol", "gol is game of life simulator and library.");

    const/* expr in gcc-12 */ auto implementations = std::vector<std::string_view>{
        "MapGOL",
	"TMapGOL"
    };

    const auto implementations_help_text = fmt::format("What game of life implementation to use. Options: {}", fmt::join(implementations, ", "));
    options.add_options()
        ("i,implementation", implementations_help_text, cxxopts::value<std::string>()->default_value("MapGOL"))
        ("f,file", "Init cells from text file", cxxopts::value<std::string>())
        ("h,help", "Print help");

    options.parse_positional({"implementation"});
    options.positional_help("implementation");
    options.show_positional_help();

    const auto result = options.parse(argc, argv);

    if (result.count("help")) {
        fmt::print("{}\n", options.help());
        return 0;
    }


    auto construct_run_options = [&]<gol::game_of_life G>() -> gol::RunOptions<G> {
        auto run_options = gol::RunOptions<G> { };

        if (result.count("file")) {
            run_options.cell_file = result["file"].as<std::string>();
        }
        return run_options;
    };


    const auto str_hash = std::hash<std::string>{ };
    const auto chosen_implementation = result["implementation"].as<std::string>();
    const auto chosen_implementation_hash = str_hash(chosen_implementation);

    if (str_hash("MapGOL") == chosen_implementation_hash) {
        gol::run_game(construct_run_options.operator()<gol::MapGOL<int>>());
    }
    else if (str_hash("TMapGOL") == chosen_implementation_hash) {
	gol::run_game(construct_run_options.operator()<gol::TMapGOL<int>>());
    }
    else {
        fmt::print("{} not implemented. Available impelentations: {}\n", chosen_implementation, fmt::join(implementations, "\n"));
    }

}
