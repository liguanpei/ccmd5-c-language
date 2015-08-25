/* *****************************************************************************
 *  Filename:    ccmd5.c
 *  Author:      ligp
 *  Data:        2015-08-24
 *  Version:     1.0
 *  Function:    get cc md5 key
 *  Running:     ./ccmd5
 *  Email:       ligp@bokecc.com
 * ****************************************************************************/

#include <stdlib.h>  
#include <string.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include "md5.h"
#include "cckey.h"

#define MD5_HASHBYTES 16
#define UVID_MAX_LEN  32
#define MD5_KEY_LEN  8
#define CC_TOKEN "BokeCC"

#define DEBUG 0

int cc_md5(char *src, int len, char key[])
{
	int i;  
	char tmp[3] = {0};
	unsigned char sout[MD5_HASHBYTES] = {0};  

	CC_MD5_CTX ctx;  
	memset(&ctx, 0, sizeof(CC_MD5_CTX));  
						  
	CC_MD5_Init(&ctx);
	CC_MD5_Update(&ctx, (unsigned char *)src, len);  
	CC_MD5_Final((unsigned char *)sout, &ctx);  
									  
	bzero(key, sizeof(key));
	for(i = 0; i < MD5_HASHBYTES; i++)  
	{ 
		sprintf(tmp, "%02X", sout[i]);
		strcat(key, tmp); 
	}  
	if(strlen(key) != MD5_HASHBYTES*2) 
	{
		return -1;
	}
	
	return 0;
}

int cc_getkey(char *uid, int uid_len, char *vid, int vid_len, char key[], int key_len)
{
	char tmp[UVID_MAX_LEN*3];
	char result[MD5_HASHBYTES*2+1]; 

	bzero(result, sizeof(result));
	if(cc_md5(vid, vid_len, result) != 0)
	{
		fprintf(stderr, "[error] cc get key1 error\n");
		return -1;
	}
	if(DEBUG)
		printf("result = %s\n", result);

	bzero(tmp, sizeof(tmp));
	snprintf(tmp, MD5_HASHBYTES*2+1+uid_len+1,"%s|%s", result, uid);
	
	bzero(result, sizeof(result));
	if(cc_md5(tmp, strlen(tmp), result) != 0)
	{
		fprintf(stderr, "[error] cc get key2 error\n");
		return -1;
	}
	if(DEBUG)
		printf("result2 = %s\n", result);

	bzero(tmp, sizeof(tmp));
	snprintf(tmp, MD5_HASHBYTES*2+1+strlen(CC_TOKEN)+1,"%s|%s", result, CC_TOKEN);
	
	bzero(result, sizeof(result));
	if(cc_md5(tmp, strlen(tmp), result) != 0)
	{
		fprintf(stderr, "[error] cc get key3 error\n");
		return -1;
	}
	if(DEBUG)
		printf("result3 = %s\n", result);
	
	memset(key, 0, key_len);
	strncpy(key, result, MD5_KEY_LEN);

	return 0;
}
  
int main(int argc, char **argv)  
{
	char result[9];
	char *uid = "1936D297411C3A27";
	int uid_len = 16;
	char *vid = "46862CF0F9D672AC9C33DC5901307461";
	int vid_len = 32;
	int err = 0;

	err = cc_getkey(uid, uid_len, vid, vid_len, result, sizeof(result));
	if(err != 0) 
	{
		return -1;
	}
	printf("result = %s\n", result);
	
	return 0;  
}  
