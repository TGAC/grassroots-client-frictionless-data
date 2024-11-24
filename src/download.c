/*
 * download.c
 *
 *  Created on: 25 Oct 2024
 *      Author: billy
 */


#include "string_utils.h"
#include "json_util.h"



char *GetRootURL (const char *full_url_s, const char *package_name_s)
{
  char *final_package_s = Strrstr (full_url_s, package_name_s);

  if (final_package_s)
  	{
  		const size_t len = final_package_s - full_url_s;
  		char *root_url_s = CopyToNewString (full_url_s, len, false);

  		if (root_url_s)
  			{
  				return root_url_s;
  			}

  	}		/* if (final_package_s) */

  return NULL;
}


int DownloadResource (json_t *resource_p, CurlTool *curl_p, const char * const root_url_s, const char * const output_dir_s)
{
	int res = 0;

	long size = 0;

	if (GetJSONLong (resource_p, "bytes", &size))
		{
			const char *path_s = GetJSONString (resource_p, "path");

			if (path_s)
				{
					const char *checksum_s = GetJSONString (resource_p, "checksum");
					char *url_s = ConcatenateStrings (root_url_s, path_s);

					if (url_s)
						{
							char *output_file_s = ConcatenateStrings (output_dir_s, path_s);

							if (output_file_s)
								{
									bool b = DownloadFile (curl_p, url_s, output_file_s);

									if (b)
										{

										}

									FreeCopiedString (output_file_s);
								}

							FreeCopiedString (url_s);
						}

				}		/* if (path_s) */

		}		/* if (GetJSONLong (resource_p, "bytes", &size)) */


	return res;
}



#include <openssl/evp.h>

// OpenSSL engine implementation
#define OPENSSL_ENGINE NULL


/**
 * Returns the SHA256 value of the input string
 *
 * @param string input string for which the hash to be calculated
 * @returns string (32 bytes) - SHA256 hash
 */
static unsigned char *GetChecksum (const unsigned char *input_s, const EVP_MD *method_p)
{
	unsigned char *output_s = NULL;
	EVP_MD_CTX *ctx_p = EVP_MD_CTX_new ();

	if (ctx_p)
		{
	    int res = EVP_DigestInit_ex (ctx_p, method_p, NULL);

	    if (res == 1)
	    	{
	        // Hashes cnt bytes of data at d into the digest context mdCtx
	    		const size_t input_length  = strlen ((const char *) input_s);

	    		res = EVP_DigestUpdate (ctx_p, input_s, input_length);


	    		if (res == 1)
	    			{
	    				unsigned char checksum [EVP_MAX_MD_SIZE] =  { 0 };
	    				unsigned int checksum_length = 0;

	    				res = EVP_DigestFinal_ex (ctx_p, checksum, &checksum_length);

	    				if (res == 1)
	    					{
	    						output_s = (unsigned char *) AllocMemory ((checksum_length + 1) * sizeof (unsigned char));

	    						if (output_s)
	    							{
	    								memcpy (output_s, checksum, checksum_length);
	    							}

	    					}
	    				else
	    					{
	    					}
	    			}
	    		else
						{
	  	        printf ("EVP_DigestUpdate () failed, error 0x%lx\n", ERR_get_error ());
						}

	    	}
	    else
	    	{
	        printf ("EVP_DigestInit_ex () failed, error 0x%lx\n", ERR_get_error ());
	    	}

	    EVP_MD_CTX_free (ctx_p);
		}
	else
		{
      printf ("EVP_MD_CTX_new () failed, error 0x%lx\n", ERR_get_error ());
		}

    return output_s;
}


static unsigned char *GetSHA256Checksum (const unsigned char *input_s)
{
	return GetChecksum (input_s, EVP_sha256 ());
}


