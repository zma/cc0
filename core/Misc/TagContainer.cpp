#include "TagContainer.h"

TagContainer::TagContainer()
{

}

TagContainer::~TagContainer()
{

}

void* TagContainer::GetTag(std::string name)
{
    std::map<std::string, void *>::iterator it = _tags.find(name);
    if (it == _tags.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

void TagContainer::SetTag(std::string name, void* value)
{
    _tags[name] = value;
}
