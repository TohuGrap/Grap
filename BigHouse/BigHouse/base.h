

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

	static bool CopyFileIntoFolder(const char* src_path, const char* des_path) {

		std::ifstream::pos_type size;
		char * memblock;

		std::ifstream file_src (src_path, std::ios::in|
																	std::ios::binary|
																	std::ios::ate);
		if (file_src == NULL) {
		  return false;
		}
		if (file_src.is_open()) {
			size = file_src.tellg();
			memblock = new char [size];
			file_src.seekg (0, std::ios::beg);
			file_src.read (memblock, size);
			file_src.close();
		}

		std::fstream file_des(des_path, std::ios::out | std::ios::binary);
		file_des.seekg(0, std::ios::beg);  
		file_des.write(memblock, size);

		file_des.close();
		return true;
	}

	static bool RemoveFile(const char* file_path) {
		int check = remove(file_path);
		if (check == 0)
			return true;
		else 
			return false;
	}

	static bool CopyFiles(const wchar_t * src_path, const wchar_t* dest_path) {  
    bool ret = CopyFile(src_path, dest_path, FALSE);
	  return ret;
	}
	static bool ChecIsExistFile(const char* path) {
   bool check = access(path, 0);
	 if (check == -1)
		 return false;
	 else
		 return true;
	}

	static CString RemoveExtensionFile(CString input_file) {
    CString str = input_file;
		str = str.Left(str.ReverseFind('.'));
		return str;
	}
}

#endif // BASE_H_