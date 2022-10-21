#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string_view>

class Memory_Manager {
private:
	std::uintptr_t PID = 0;
	void* handler = nullptr;
public:
	Memory_Manager(
		const std::string_view PROC
	) {
		::PROCESSENTRY32 Windows_Proc_Entry32 = {};
		Windows_Proc_Entry32.dwSize = sizeof(::PROCESSENTRY32);
		const auto ProcessSnapShot = ::CreateToolhelp32Snapshot(
			TH32CS_SNAPPROCESS, 0
		);
		while (
			::Process32Next(ProcessSnapShot, &Windows_Proc_Entry32)
			) {
			if (
				!PROC.compare(Windows_Proc_Entry32.szExeFile)
				) {
				PID = Windows_Proc_Entry32.th32ProcessID;
				handler = ::OpenProcess(PROCESS_ALL_ACCESS, false, PID); 
			}
		}
		if (ProcessSnapShot) {
			::CloseHandle(ProcessSnapShot);
		}
	}

	~Memory_Manager() {
		if (
			handler
			) {
			::CloseHandle(handler);
		}
	}

	const std::uintptr_t Load_Module_Address(
		const std::string_view mod
	) const noexcept {
		std::uintptr_t res = 0;
		::MODULEENTRY32  Windows_Proc_Entry32 = {};
		Windows_Proc_Entry32.dwSize = sizeof(::MODULEENTRY32);
		const auto  ProcessSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
		while (
			::Module32Next(ProcessSnapShot, &Windows_Proc_Entry32)
			) {
			if (
				!mod.compare(Windows_Proc_Entry32.szModule)
				) {
				res = reinterpret_cast<std::uintptr_t>(Windows_Proc_Entry32.modBaseAddr);
				break;
			}
		}
		if (ProcessSnapShot) {
			::CloseHandle(ProcessSnapShot);
		}
		return res;
	}
	template<typename T> T R(SIZE_T address) {T buffer;::ReadProcessMemory(handler, (LPCVOID)address, &buffer, sizeof(T), NULL);return buffer;}
	template<typename T> void W(SIZE_T address, T buffer) { ::WriteProcessMemory(handler, (LPVOID)address, &buffer, sizeof(buffer), NULL); } // // This code was a re moded and re distriubuted version from a collection of game cheats for CSGO. This Memory module can be used but if you plan to start selling cheats or selling applications that use this module it is not really of best interest to do this. Thank you to Heath for giving a perfect example of process reading, writing and initilizing

};
