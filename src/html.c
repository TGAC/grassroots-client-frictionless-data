/*
** Copyright 2014-2020 The Earlham Institute
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
/*
 * html.c
 *
 *  Created on: 6 Apr 2021
 *      Author: billy
 */

#include "html.h"

#include "programme.h"


/*
 * static declarations
 */

static bool PrintHeader (FILE *out_f, const char *title_s);

static bool PrintFooter (FILE *out_f);


/*
 * api definitions
 */

bool PrintProgrammeToHTML (const json_t *programme_json_p, const char *output_filename_s)
{
	bool result = false;
	FILE *out_f = fopen (output_filename_s, "w");

	if (out_f)
		{
			const char *title_s = GetJSONString (programme_json_p, PR_FD_NAME_S);

			if (title_s)
				{
					if (PrintHeader (out_f, title_s))
						{

							if (PrintFooter (out_f))
								{
									result = true;
								}		/* if (PrintFooter (out_f)) */

						}		/* if (PrintHeader (out_f, title_s)) */

				}		/* if (title_s) */

			fclose (out_f);
		}		/* if (out_f) */

	return result;
}


/*
 * static definitions
 */


static bool PrintProgrammeHTML (FILE *out_f, const json_t *programme_json_p)
{
	bool res = false;

	if (fprintf (out_f, "<ul>") > 0)
		{

			if (fprintf (out_f, "</ul>") > 0)
				{
					res = true;
				}		/* if (fprintf (out_f, "<ul>") > 0) */

		}		/* if (fprintf (out_f, "<ul>") > 0) */

	return res;
}


static bool PrintHeader (FILE *out_f, const char *title_s)
{
	bool res = (fprintf ("<html>\n<head>\n\t<title>%s</title>\n</head><body>\n", title_s) > 0);

	return res;
}


static bool PrintFooter (FILE *out_f)
{
	bool res = (fprintf ("</body>\n</html>\n") > 0);

	return res;
}


static bool PrintListItem (FILE *out_f, const char *key_s, const char *value_s)
{
	bool res = (fprintf ("<li><strong>%s</strong>: %s</li>\n", key_s, value_s) > 0);

	return res;
}
