#ifndef LOCATION_H
#define LOCATION_H
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

struct Location
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(FileName);
		ar & BOOST_SERIALIZATION_NVP(Line);
		ar & BOOST_SERIALIZATION_NVP(Column);
	}
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
