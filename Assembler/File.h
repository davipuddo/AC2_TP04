#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include "List.h"

class File
{
	private:
	const char* filename;

	public:

	File (const char* filename)
	{
		if (filename)
		{
			this->filename = filename;
		}
	}

	List* read (void)
	{
		List* res = new List();

		std::fstream fs (filename, std::ios::in);

		if (fs)
		{
			std::string line = "";
			while (std::getline(fs, line) )
			{
				res->insert(line.data());
			}
		}
		return (res);
	}

	void setFilename(const char* filename)
	{
		if (filename)
		{
			this->filename = filename;
		}
	}

	void write (List* list)
	{
		if (list)
		{
			std::fstream fs (filename, std::ios::out);

			if (fs)
			{
				int n = list->getSize();

				for (int i = 0; i < n; i++)
				{
					fs << list->remove();
					fs << "\n";
				}
			}
		}
	}
};

#endif
