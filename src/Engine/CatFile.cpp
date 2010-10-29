/*
 * Copyright 2010 Daniel Albano
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CatFile.h"

/**
 * Creates a CAT file stream. A CAT file starts with an index of the
 * offset and size of every file contained within. Each file consists
 * of a filename followed by its contents.
 */
CatFile::CatFile(const char *path) :
	std::ifstream(path, std::ios::in | std::ios::binary),
	_amount(0), _offset(0), _size(0)
{
	if (!this)
		return;

	// Get amount of files
	read((char*)&_amount, sizeof(_amount));
	_amount /= 2 * sizeof(_amount);

	// Get object offsets
	seekg(0, std::ios::beg);

	_offset = new unsigned int[_amount];
	_size   = new unsigned int[_amount];

	for (unsigned int i = 0; i < _amount; i++)
	{
		read((char*)&_offset[i], sizeof(*_offset));
		read((char*)&_size[i],   sizeof(*_size));
	}
}

/**
 * Frees associated memory.
 */
CatFile::~CatFile()
{
	delete[] _offset;
	delete[] _size;

	close();
}

/**
 * Load an object into memory.
 */
char *CatFile::load(unsigned int i)
{
	if (i >= _amount)
		return 0;

	seekg(_offset[i], std::ios::beg);

	// Skip filename
	char namesize;
	read(&namesize, 1);
	seekg(namesize, std::ios::cur);

	// Read object
	char *object = new char[_size[i]];
	read(object, _size[i]);

	return object;
}