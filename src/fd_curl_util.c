/*
 * fd_curl_util.c
 *
 *  Created on: 12 Apr 2021
 *      Author: billy
 */

#include "byte_buffer.h"
#include "curl/curl.h"


static size_t WriteMemoryCallback (char *response_data_p, size_t block_size, size_t num_blocks, void *store_p)
{
	size_t total_size = block_size * num_blocks;
	size_t result = CURLE_OK;
	ByteBuffer *buffer_p = (ByteBuffer *) store_p;

	size_t remaining_space = GetRemainingSpaceInByteBuffer (buffer_p);

	if (remaining_space < total_size)
		{
			if (!ExtendByteBuffer (buffer_p, total_size - remaining_space + 1))
				{
					result = CURLE_OUT_OF_MEMORY;
				}
		}

	if (result == CURLE_OK)
		{
			if (AppendToByteBuffer (buffer_p, response_data_p, total_size))
				{
					result = total_size;
				}
		}

	return result;
}


char *GetWebData (const char *url_s)
{
	char *data_s = NULL;
	CURL *curl_p = curl_easy_init ();
	CURLcode res;

	if (curl_p)
		{
			ByteBuffer *buffer_p = AllocateByteBuffer (1024);

			if (buffer_p)
				{
					curl_easy_setopt (curl_p, CURLOPT_URL, url_s);
					curl_easy_setopt (curl_p, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
					curl_easy_setopt (curl_p, CURLOPT_WRITEDATA, buffer_p);
					res = curl_easy_perform (curl_p);

					data_s = DetachByteBufferData (buffer_p);
				}

			curl_easy_cleanup (curl_p);
		}

	return data_s;
}
