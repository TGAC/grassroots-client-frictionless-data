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
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>
#include <fcntl.h>
#include <pwd.h>

#include <fd_filesystem_util.h>
#include "memory_allocations.h"
#include "fd_string_util.h"


static bool CreateSingleLevelDirectory (const char *path_s);



char GetFileSeparatorChar (void)
{
	return '/';
}

const char *GetCurrentDirectoryString (void)
{
	return "./";
}

const char *GetParentDirectory (void)
{
	return "../";
}

const char *GetPluginPattern (void)
{
	return "*.so";
}


bool IsPathValid (const char * const path)
{
	struct stat st;

	return (stat (path, &st) == 0);
}


bool IsPathAbsolute (const char * const path)
{
	if (path)
		{
			return (*path == '/');
		}
	return false;
}

/*
char *GetFilenameOnly (const char * const path)
{
	char *filename_p = strrchr (path, '/');
	if (filename_p)
		{
			++ filename_p;
			return CopyToNewString (filename_p, false);
		}

	return NULL;
}

char *GetPathOnly (const char * const full_path_to_file)
{
	char *filename_p = strrchr (full_path_to_file, '/');
	if (filename_p)
		{
			ptrdiff_t length = filename_p - full_path_to_file;

			char *path_p = (char *) AllocMemory (length + 1);
			if (path_p)
				{
					strncpy (path_p, full_path_to_file, length);
					* (path_p + length) = '\0';
					return path_p;
				}
		}

	return NULL;
}
*/




static bool CreateSingleLevelDirectory (const char *path_s)
{
	struct stat st;
	bool success_flag = false;

	if (stat (path_s, &st) == 0)
		{
			if (S_ISDIR (st.st_mode))
				{
					/* directory already exists */
					success_flag = true;
				}
			else
				{
					/* it's not a directory */
				}
		}
	else
		{
			/* Directory does not exist. EEXIST for race condition */
			const int res = mkdir (path_s, S_IRWXU);

			if (res == 0)
				{
					success_flag = true;
				}
			else
				{

				}
		}

	return success_flag;
}


/**
** mkpath - ensure all directories in path exist
** Algorithm takes the pessimistic view and works top-down to ensure
** each directory in path exists, rather than optimistically creating
** the last element and working backwards.
*/
bool EnsureDirectoryExists (const char * const path_s)
{
	bool success_flag = false;
	char *copied_path_s = CopyToNewString (path_s, 0, false);

	if (copied_path_s)
		{
			char *current_p = copied_path_s;
			char *next_p = NULL;

			success_flag = true;

	    while (success_flag && (next_p = strchr (current_p, '/')))
				{
					if (next_p != current_p)
						{
							/* Neither root nor double slash in path */
							*next_p = '\0';
							success_flag = CreateSingleLevelDirectory (copied_path_s);
							*next_p = '/';
						}

					current_p = next_p + 1;
				}		/* while (success_flag && ((next_p = strchr (current_p, '/')) != 0) */

			FreeCopiedString (copied_path_s);
		}		/* if (copied_path_s) */


	if (success_flag)
		{
			success_flag = CreateSingleLevelDirectory (path_s);
		}

	return success_flag;
}




/**
 * Copy the contents of one named file to another.
 *
 * @param src_filename_s The name of the the source file.
 * @param dest_filename_s The name of the the source file.
 * @param callback_fn The callback_fn to denote progress (currently unused).
 * @return true on success, false on error with errno set to the appropriate value.
 */
bool CopyToNewFile (const char * const src_filename_s, const char * const dest_filename_s, void (*callback_fn) (void))
{
	FILE *in_f = fopen (src_filename_s, "rb");
	bool success_flag = false;
	int saved_errno = 0;

	if (in_f)
		{
			FILE *out_f = fopen (dest_filename_s, "wb");

			if (out_f)
				{
					char buffer [8192];		/* 32 * 256 */
					success_flag = true;

					while (success_flag)
						{
							size_t num_objs = fread (buffer, 32, 256, in_f);

							if (num_objs > 0)
								{
									if (fwrite (buffer, 32, 256, out_f) != num_objs)
										{
											success_flag = false;
											saved_errno = errno;
										}
								}
						}		/* while (success_flag) */

					if (success_flag)
						{
							success_flag = (feof (in_f) != 0) ? true : false;
						}

					fclose (out_f);

				}		/* if (out_f) */

			fclose (in_f);

		}		/* if (in_f) */

	/* In case closing one of the files overwrote it, restore errno */
	if (saved_errno != 0)
		{
			errno = saved_errno;
		}

	return success_flag;
}


char *GetHomeDirectory (void)
{
	char *result_s = NULL;
	const char *home_s = getenv ("HOME");

	if (home_s == NULL)
		{
			uid_t user = getuid ();

			if (user)
				{
					struct passwd *password_p =  getpwuid (user);

					if (password_p)
						{
							home_s = password_p -> pw_dir;
						}
				}
		}

	if (home_s)
		{
			result_s = CopyToNewString (home_s, 0, false);

			if (!result_s)
				{

				}
		}

	return result_s;
}


bool SetCurrentWorkingDirectory (const char * const path)
{
	return (chdir	(path) == 0);
}


char *GetCurrentWorkingDirectory (void)
{
	long buffer_length = 128;
	char *buffer_s = (char *) AllocMemory (buffer_length);
	char *path_s = NULL;

	while (!path_s && buffer_s)
		{
			path_s = getcwd (buffer_s, buffer_length);

			if (!path_s)
				{
					FreeMemory (buffer_s);

					if (errno == ERANGE)
						{
							buffer_length <<= 1;
							buffer_s = (char *) AllocMemory (buffer_length);
						}
					else
						{
							buffer_s = NULL;
						}
				}
		}

	return path_s;
}


bool IsDirectory (const char * const path_s)
{
	struct stat buf;

	if (stat (path_s, &buf) == 0)
	  {
			return (S_ISDIR (buf.st_mode));
		}

	return false;
}



bool RemoveFile (const char * const path_s)
{
	return (unlink (path_s) == 0);
}

