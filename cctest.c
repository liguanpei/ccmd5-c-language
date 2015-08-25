#include <stdio.h>
#include <cckey.h>

int main(int argc, char **argv)  
{
	char result[9];
	//char *uid = "1936D297411C3A27222222222222222222";
	char *uid = "1936D297411C3A27";
	int uid_len = 16;
	//char *vid = "46862CF0F9D672AC9C33DC59013074611111111111111111";
	char *vid = "78FCE694160CB1229C33DC5901307461";
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
