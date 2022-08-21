#include "leaderboard.hpp"
#include <cassert>
#include <sstream>
#include <fstream>

std::string Leaderboard::toString() const 
{
    std::ostringstream output;
    
    int i = 1;
    for (auto beg = table.crbegin(); beg != table.crend(); ++beg) {
        output << i << ". " << beg->second << " - " << beg->first << std::endl;
        ++i;
    }
    
    return output.str();
}

void Leaderboard::read() {
    std::ifstream ift(kLeaderboardFilename);

    std::string         line;
    tTable::key_type    key;
    tTable::mapped_type val;

    while (std::getline(ift, line)) {
        std::istringstream iss(line);
        iss >> val;
        assert(iss.fail());
        iss >> key;
        assert(iss.fail());
        table.insert({key, val});
    }
}

void Leaderboard::write() const
{
    std::ofstream output(kLeaderboardFilename);
    for (auto el : table)
        output << el.second << " " << el.first << std::endl;
}
