#ifndef LOCATION_H
#define LOCATION_H
#include <string>

struct Location
{
public:
    std::string FileName;
    int Line;
    int Column;
    Location();
//     Location(int line, int column);
//     Location(const Location &other);
// public:
//     int ComparesTo(const Location& other) const;
// public:
//     virtual Location& operator=(const Location& other);
//     virtual bool operator==(const Location& other) const;
//     bool operator!=(const Location& other) const;
//     bool operator>(const Location& other) const;
//     bool operator>=(const Location& other) const;
//     bool operator<(const Location& other) const;
//     bool operator<=(const Location& other) const;
};

#endif // LOCATION_H
