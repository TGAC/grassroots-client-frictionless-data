/*
 * fd_filesystem_util.c
 *
 *  Created on: 13 Apr 2021
 *      Author: billy
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "fd_filesystem_util.h"
#include "fd_string_util.h"



char *MakeFilename (const char * const path, const char * const filename)
{
	const size_t path_length = strlen (path);
	const char FS_SEPARATOR = GetFileSeparatorChar ();
	bool needs_separator_flag = false;
	size_t name_length = path_length + strlen (filename) + 1;
	char *name_p = NULL;

	if (path_length > 0)
		{
			const char c = * (path + path_length - 1);

			if ((c != FS_SEPARATOR) && (c != '/'))
				{
					needs_separator_flag = true;
					++ name_length;
				}
		}

	//printf ("path: \"%s\", filename: \"%s\" name_length: %lu\n", path, filename, name_length);

	name_p = (char *) malloc (name_length);
	if (name_p)
		{
			char *ptr = name_p;

			strncpy (ptr, path, path_length);
			ptr += path_length;

			if (FS_SEPARATOR != '/')
				{
					char *c_p = name_p;

					while ((c_p = strchr (c_p, '/')) != NULL)
						{
							*c_p = FS_SEPARATOR;
						}
				}

			if (needs_separator_flag)
				{
					*ptr = FS_SEPARATOR;
					++ ptr;
				}

			strcpy (ptr, filename);

			return name_p;
		}

	return NULL;
}





char *GetOutputFilename (const char *dir_s, const char *name_s, const char *extension_s)
{
	char *filename_s = NULL;
	char *copied_name_s = ConcatenateVarargsStrings (name_s, ".", extension_s, NULL);

	if (copied_name_s)
		{
			/*
			 * Replace any non file-system characters to
			 * make sure that it is a valid filename.
			 * The safest approach is to replace all
			 * non-alphanumeric characters with an
			 * underscore.
			 */
			char *c_p = copied_name_s;
			size_t i = strlen (name_s);

			for ( ; i > 0; -- i, ++ c_p)
				{
					if (isalnum (*c_p) == 0)
						{
							*c_p = '_';
						}
				}

			if (dir_s)
				{
					if (EnsureDirectoryExists (dir_s))
						{
							filename_s = MakeFilename (dir_s, copied_name_s);
						}
					else
						{
							fprintf (stderr, "Failed to create output directory \"%s\"\n", dir_s);
						}

					FreeCopiedString (copied_name_s);
				}		/* if (dir_s) */
			else
				{
					filename_s = copied_name_s;
				}

		}		/* if (copied_name_s) */

	return filename_s;
}


