

#ifndef BASE_H_
#define BASE_H_
#include "stdafx.h"
#include <io.h>
namespace Base {
  static std::string WcharToString(wchar_t* wchar_str) {
    std::string str("");
    int index = 0;
    while (wchar_str[index] != 0) {
      str += (char)wchar_str[index];
      ++ index;
    }
    return str;
  }

  static wchar_t* StringToWchar(std::string str) {
    int index = 0;
    int count = str.size();
    wchar_t *ws_str = new wchar_t [count+1]; 
    while (index < str.size()) {
      ws_str[index] = (wchar_t)str[index];
      index++;
    }
    ws_str[index] = 0;
    return ws_str;
  }

  static std::vector<std::string> ListFileInFolder(std::string path_folder) {
    WIN32_FIND_DATA find_file_data;
    std::vector<std::string> list_file;
    wchar_t *path_folder_full = StringToWchar(path_folder);
    HANDLE hFind = FindFirstFile(path_folder_full, &find_file_data);
    list_file.push_back(WcharToString(find_file_data.cFileName));
    while(FindNextFile(hFind, &find_file_data)) {
      list_file.push_back(WcharToString(find_file_data.cFileName));
    }
    return list_file;
  }

  static CString GetPathModule() {
    CString full_path = L"";
    ::GetModuleFileName(NULL, full_path.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
    full_path = full_path.Left(full_path.ReverseFind('\\'));
    return full_path;
  }

	static char* CStringToChar(CString str) {
		char * chstr = new char [MAX_PATH];
		long nsize = str.GetLength();
		memset(chstr, 0, nsize);
		wcstombs(chstr, str, nsize+1);
		return chstr;
	}
}



#endif // BASE_H_