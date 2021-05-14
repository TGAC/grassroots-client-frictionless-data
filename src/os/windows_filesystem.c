/*
** Copyright 2014-2016 The Earlham Institute
** 
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
** 
**     http://www.apache.org/licenses/LICENSE-2.0
** 
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdarg.h>
#include <string.h>

#include <windows.h>
#include <WinBase.h>
#include <Dbghelp.h>

#include "fd_filesystem_util.h"
#include "memory_allocations.h"
#include "fd_string_util.h"




char GetFileSeparatorChar (void)
{
	return '\\';
}

const char *GetCurrentDirectoryString (void)
{
	return ".\\";
}

const char *GetParentDirectory (void)
{
	return "..\\";
}

const char *GetPluginPattern (void)
{
	return "*.dll";
}


BOOLEAN IsPathValid (const char * const path)
{
	DWORD attrs = GetFileAttributes (path);
	return ((attrs | FILE_ATTRIBUTE_DIRECTORY) != 0);
}


BOOLEAN IsPathAbsolute (const char * const path_s)
{
	BOOLEAN absolute_path_flag = FALSE;

	if (path_s)
		{
			const size_t len = strlen (path_s);

			if (len >= 2)
				{
					if (strncmp ("\\\\", path_s, 2) == 0)
						{
							absolute_path_flag = TRUE;
						}
					else if ((len >= 3) && (isalpha (*path_s)) && (strncmp (":\\", path_s + 1, 2) == 0))
						{
							absolute_path_flag = TRUE;
						}
				}
		}

	return absolute_path_flag;
}

BOOLEAN EnsureDirectoryExists (const char * const path_s)
{
	return MakeSureDirectoryPathExists (path_s);
}




BOOLEAN IsDirectory (const char * const path)
{
	DWORD dw = GetFileAttributes (path);

	return (dw & FILE_ATTRIBUTE_DIRECTORY);
}



/*

STRING *WindowsFileSystem :: makeLibName (const STRING &str)
{
	return new STRING (str + ".dll");
}

STRING *WindowsFileSystem :: makeFilename (const STRING &path, const STRING &filename)
{
	const uint32 path_length = path.length ();
	const uint32 filename_length = filename.length ();
	const STRING &parent_dir = getParentDirectory ();
	const uint32 parent_length = parent_dir.length ();

	uint32 parent_count = 0;
	uint32 i = 0;
	STRING sub;

	// does filename use ..\ ?
	while ((filename.compare (i, parent_length, parent_dir)) == 0)
		{
			i += parent_length;
			++ parent_count;
		}


	if (parent_count)
		{
			uint32 j = path_length - 1; // ignore last char as if sep is irrelevant
			uint32 sep_count = 0;
			char sep = getSeparator ();

			for ( ; j > 0; -- j)
				{
					if (path [j] == sep)
						{
							++ sep_count;

							if (sep_count == parent_count)
								{
									break;
								}
						}
				}

			if (sep_count == parent_count)
				{
					STRING *s_p = new STRING (path.substr (0, j + 1));

					*s_p += filename.substr (i, parent_length - i);

					return s_p;
				}
			else
				{
					(*getErrorStream ())  << "WindowsFileSystem :: makeFilename. Impossible to resolve path " << path << " and filename " << filename << endl;
					return 0;
				}
		}
	else
		{
			switch (path_length)
				{
					case 0:
						{
							return new STRING (filename);
						}
					break;	// case 0

					case 1:
						{
							if (isalpha (path [0]))
								{
									return new STRING (path + ":\\" + filename);
								}
							else
								{
									return new STRING (path + "\\" + filename);
								}
						}
					break;	// case 1

					case 2:
						{
							if ((isalpha (path [0])) && (path [1] == ':'))
								{
									return new STRING (path + "\\" + filename);
								}
							else
								{
									return new STRING (path + "\\" + filename);
								}
						}
					break;	// case 2


					default:
						{
							if (path [path_length - 1] != '\\')
								{
									return new STRING (path + '\\' + filename);
								}
							else
								{
									return new STRING (path + filename);
								}
						}
					break;	// default
				}
		}

	(*getErrorStream ())  << "makeFilename: unable to resolve Path " << path << " " << filename << endl;
	return 0;
}

LOGICAL WindowsFileSystem :: ensurePathValid (const STRING &path)
{
	return FALSE;
}

void WindowsFileSystem :: setCurDir (const STRING & path)
{
	char * dir=_getcwd(NULL,1024);
	afs_old_cur_dir=dir;
	free(dir);
	_chdir(path.c_str());
}


void WindowsFileSystem :: getCurDir (STRING &path)
{
	path = getcwd(NULL,1024);
}

void WindowsFileSystem :: restoreCurDir ()
{
	_chdir(afs_old_cur_dir.c_str());
}

LOGICAL WindowsFileSystem::isAbsolute (const STRING &path)
{
	if (isalpha(path.at(0)) && (path.substr(1,2)==":\\"))
		return TRUE;

	return FALSE;
}


void WindowsFileSystem :: appendSeparator (STRING &str) const
{
	const uint32 length = str.length ();

	switch (length)
		{
			case 1:

			break;

			case 2:
				if (isalpha (str [0]))
					{
						str += '\\';
					}
			break;

			default:
				if (str [length - 1] != '\\')
					{
						str += '\\';
					}
			break;
		}
}
*/


bool RemoveFile (const char * const path_s)
{
	return (DeleteFile (path_s) != 0);
}
