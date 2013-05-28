
#include "Location.h"

Location::Location()
{
    this->FileName = "";
    this->Line = 0;
    this->Column = 0;
}


/*
int Location::ComparesTo(const Location& other) const
{
    if (this->FileName != other.FileName)
    {
        return 0;
    }
    
    int diff = this->Line - other.Line;
    if (diff != 0)
    {
        return diff;
    }
  
    diff = this->Column - other.Column;
    
    return diff;
}

Location& Location::operator=(const Location & other)
{
    this->Line = other.Line;
    this->Column = other.Column;
    return *this;
}


bool Location::operator==(const Location& other) const
{
    return (this->ComparesTo(other) == 0);
}

bool Location::operator!=(const Location& other) const
{
    return (this->ComparesTo(other) != 0) || (this->FileName != other.FileName); 
}

bool Location::operator>(const Location& other) const
{
    return (this->FileName == other.FileName) &&(this->ComparesTo(other) > 0); 
}
bool Location::operator>=(const Location& other) const
{
    return  (this->FileName == other.FileName) &&(this->ComparesTo(other) >= 0); 
}
bool Location::operator<(const Location& other) const
{
    return (this->FileName == other.FileName) &&(this->ComparesTo(other) < 0); 
}
bool Location::operator<=(const Location& other) const
{
    return (this->FileName == other.FileName) &&(this->ComparesTo(other) <= 0); 
}*/
