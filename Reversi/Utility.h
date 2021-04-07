#pragma once

#include <fstream>
#include <cassert>

namespace util
{
	template <class T>
	void saveFile(const char path[], const T& out)
	{
		std::ofstream ofs(path, std::ios::out | std::ios::binary);

		char buffer[sizeof(T)];

		memcpy_s(buffer, sizeof(T), &out, sizeof(T));

		if (ofs.is_open())
		{
			size_t size = sizeof(T);
			size_t i = 0;
			while (i < size)
			{
				ofs.put(buffer[i]);
				++i;
			}
			ofs.close();
		}
		else
		{
			assert(L"out fail" && false);
		}
	}


	template <class T>
	void loadFile(const char path[], T& in)
	{


		std::ifstream ifs(path, std::ios::in | std::ios::binary);

		char buffer2[sizeof(T)];

		if (ifs.is_open())
		{
			size_t c = 0;
			size_t size = sizeof(T);
			while (c < size)
			{
				buffer2[c] = ifs.get();
				++c;
			}
			ifs.close();

			memcpy_s(&in, sizeof(T), buffer2, sizeof(T));
		}
		else
		{
			saveFile(path, in);
		}
	}
}
