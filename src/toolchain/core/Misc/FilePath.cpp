#include "FilePath.h"

::std::string ReplaceFilePathExtension(const ::std::string& path, const ::std::string& new_ext) {
    return GetFileDirectory(path) + GetFileNameWithoutExtension(path) + new_ext;
}

::std::string ReplaceFileNameExtension(const ::std::string& path, const ::std::string& new_ext) {
    return GetFileNameWithoutExtension(path) + new_ext;
}

::std::string GetFileExtension(const ::std::string& path) {
    ::std::string file_name(GetFileName(path));
    size_t dot_pos = file_name.find_last_of('.');
    if (dot_pos == ::std::string::npos) {
        return ::std::string();
    }
    return ::std::string(file_name, dot_pos);
}

::std::string GetFileNameWithoutExtension(const ::std::string& path) {
    ::std::string file_name(GetFileName(path));
    size_t dot_pos = file_name.find_last_of('.');
    if (dot_pos == ::std::string::npos) {
        return file_name;
    }
    return ::std::string(file_name.c_str(), dot_pos);
}

::std::string GetFileName(const ::std::string& path) {
    size_t slash_pos = path.find_last_of('/');
    if (slash_pos == ::std::string::npos) {
        return path;
    }
    return ::std::string(path, slash_pos + 1);
}

::std::string GetFileDirectory(const ::std::string& path) {
    size_t slash_pos = path.find_last_of('/');
    if (slash_pos == ::std::string::npos) {
        return ::std::string();
    }
    return ::std::string(path.c_str(), slash_pos + 1);
}

::std::string ConstructFilePath(const ::std::string& dir, const ::std::string& name) {
    if (dir.size()) {
        if (dir[dir.size() - 1] == '/') {
            return dir + name;
        } else {
            return dir + "/" + name;
        }
    } else {
        return name;
    }
}
