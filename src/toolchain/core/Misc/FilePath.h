#ifndef CC0_FILE_PATH_SUPPORT_H
#define CC0_FILE_PATH_SUPPORT_H

#include <string>

#ifndef __linux__
#error "Currently the FilePath library only works on Linux!"
#else

::std::string ReplaceFileExtension(const ::std::string& path, const ::std::string& new_ext);
::std::string GetFileExtension(const ::std::string& path);
::std::string GetFileNameWithoutExtension(const ::std::string& path);
::std::string GetFileName(const ::std::string& path);
::std::string GetFileDirectory(const ::std::string& path);

#endif

#endif
