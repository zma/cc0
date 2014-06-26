#ifndef CC0_FILE_PATH_SUPPORT_H
#define CC0_FILE_PATH_SUPPORT_H

#include <string>
#include <stdexcept>
#include <unistd.h>
#include <iostream>

#ifndef __linux__
#error "Currently the FilePath library only works on Linux!"
#else

::std::string ReplaceFilePathExtension(const ::std::string& path, const ::std::string& new_ext);
::std::string ReplaceFileNameExtension(const ::std::string& path, const ::std::string& new_ext);
::std::string GetFileExtension(const ::std::string& path);
::std::string GetFileNameWithoutExtension(const ::std::string& path);
::std::string GetFileName(const ::std::string& path);
::std::string GetFileDirectory(const ::std::string& path);
::std::string ConstructFilePath(const ::std::string& dir, const ::std::string& name);

template<class ForwardIt>
::std::string FindFileInDirs(const ::std::string& file_name, ForwardIt dirs_begin, ForwardIt dirs_end) {
    for (ForwardIt i = dirs_begin; i != dirs_end; ++i) {
        ::std::string file_path(ConstructFilePath(*i, file_name));
        if (!access(file_path.c_str(), F_OK)) {
            return file_path;
        }
    }
    throw ::std::runtime_error(file_name + " not found");
}

template<class ForwardIt>
void SplitAndStorePath(const ::std::string& paths, ForwardIt it) {
    size_t split_pos;
    size_t prev_pos = 0;
    for (;;) {
        split_pos = paths.find_first_of(':', prev_pos);
        if (split_pos == ::std::string::npos) {
            *it = ::std::string(paths, prev_pos);
            break;
        } else {
            *(it++) = ::std::string(paths, prev_pos, split_pos - prev_pos);
            prev_pos = split_pos + 1;
            if (prev_pos == paths.size()) {
                return;
            }
        }
    }
}
#endif

#endif
