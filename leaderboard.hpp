#include <map>
#include <string>

class Leaderboard {
public:
    typedef std::multimap<unsigned int, std::string> tTable;

private:
    static constexpr char kLeaderboardFilename[] = "leaderboard";

    tTable table;

public:
    Leaderboard() { read(); }
    ~Leaderboard() { write(); }

    void insert(const tTable::value_type &val)
    {
        table.insert(val);
        write();
    }
    void insert(const tTable::mapped_type &name, tTable::key_type score) { insert({score, name}); }

    void clear() { table.clear(); };

    std::string toString() const;

private:
    // Write from table to file
    void write() const;
    // Read from file to table
    void read();
};