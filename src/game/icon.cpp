
#include "icon.h"
#include <exception>
#include <string>

namespace Game {
	auto IconResource::getCurrentModule() -> HMODULE {
		HMODULE hModule = nullptr;

		GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			(LPCTSTR)getCurrentModule,
			&hModule);

		return hModule;
	}

	IconResource::IconResource(i32 id) : data(nullptr), size(0) {
		auto err = [](const char* action) {
			auto str = std::string("FAILED! To ") + action + " Resource - Error Code: " + std::to_string(GetLastError());
			throw std::exception(str.c_str());
		};
		
		auto hExe = getCurrentModule();

		auto hRes = FindResourceEx(hExe, RT_ICON, MAKEINTRESOURCE(id),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
		
		if (hRes == nullptr)
			err("Find");
		
		auto hGlob = LoadResource(hExe, hRes);
		if (hGlob == nullptr)
			err("Load");

		size = SizeofResource(hExe, hRes);

		data = static_cast<u8*>(LockResource(hGlob));
		if (data == nullptr)
			err("Lock");

		///////////////////////////
		
		HANDLE hFile = CreateFile(L"test.ico", GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		DWORD read = 0;

		!WriteFile(hFile, data, size, &read, NULL);
	
		CloseHandle(hFile);
	}

	IconResource::~IconResource() {}

	auto IconResource::getData() const -> u8* {
		return data;
	}

	auto IconResource::getSize() const -> i32 {
		return size;
	}
}
