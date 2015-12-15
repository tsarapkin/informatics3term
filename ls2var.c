#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <limits.h>
#include <stdlib.h>


int main(int argc, const char** argv, const char** envp)
{
	DIR* dir;
	struct dirent* curr_file_info;
	struct stat sb;
	char* full_path = NULL;
	struct passwd  *pw_d;

	if(argc < 2){
		printf("No arguments supplied\n");
		return -1;
	}

	dir = opendir(argv[1]);

	if(dir == NULL){
		printf("Error - %d\n", errno);
		return -2;
	}

	full_path = realpath(argv[1], NULL);

	while( ( curr_file_info = readdir(dir) ) != NULL )
	{
		printf("Path||Size||Owner||Mode(octal)||File create time\n");
		printf("%s/%s||", full_path, curr_file_info->d_name);
		full_path = realpath(argv[1], NULL);
		char* tempor = full_path;
		tempor = strcat(tempor, "/");
		tempor = strcat(tempor, curr_file_info->d_name);
		if (stat(tempor, &sb) == 0) {
        	printf("Statistics couldn't be taken.");
        	return -2;
        	}
    	printf("%lld bytes||", (long long) sb.st_size);
        pw_d = getpwuid ( sb.st_uid );
  		printf("%s||", pw_d->pw_name);
    	printf("%lo||", (unsigned long) sb.st_mode);
        printf("%s\n", ctime(&sb.st_ctime));
	}

	free(full_path);
	closedir(dir);
	return 0;
}
