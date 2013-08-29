#pragma once
#include <string>
#include <map>
#include <typeinfo>

class TagContainer
{
private:
    std::map<std::string, void *> _tags;
    
public:
    TagContainer();
    virtual ~TagContainer();
    
public:
    void *GetTag(std::string name);
    void SetTag(std::string name, void *value);
    
public:
    template <typename T> T* GetTag()
    {
        return reinterpret_cast<T*>(GetTag(typeid(T).name()));
    }

    template <typename T> void SetTag(T* value)
    {
        void *data = reinterpret_cast<void *>(value);
        _tags[typeid(T).name()] = data;
    }

};

