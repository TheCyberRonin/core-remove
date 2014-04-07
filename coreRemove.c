/*********************************************************************************
 * Ruben Swyers                                                                  *
 * Recursively deletes any cores in a directory, it will go into subdirectories	 *
 *********************************************************************************
 */
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>

//Prototypes
//Prints out the contents of a directory(including subdirectories) It's recursive
void list_dir(char * name);
//Helper functions for list_dir
//checks to see if an entry is a file (returns 1 if it is, 0 if it isn't)
int checkFile(struct dirent * entry);
//checks to see if an entry is a directory(returns 1 if it is, 0 if it isn't)
int checkDir(struct dirent * entry);

int main(int argc, char *argv[])
{
	//Usage clause:
	//Expecting one command line argument, either a file name or a directory name
	if(argc != 2)
	{
		printf("usage: %s <filename>\n",argv[0]);
	}
	
	//variables
	struct stat statS;

	stat(argv[1],&statS);
	if(S_ISREG(statS.st_mode))
	{
		printf("usage: <%s> <directory> or <%s>", argv[0], argv[0]);
	}
	else if(S_ISDIR(statS.st_mode))
	{
		list_dir(argv[1]);
	}
	else
	{
		printf("The command line argument was not a file or directory\n");
	}

	return 0;
}
//Name: list_dir
//Parameters: name of the directory
//Description: prints out the contents of a directory recursively
void list_dir(char * name)
{
    DIR *dir;
    struct dirent *entry;
    int sentinel = 1;
    //Opens the directory
    if (!(dir = opendir(name)))
    {
        return;
    }
    //Processes each thing in the directory, if its a file, it prints out the data, if its a directory
    //it calls list_dir for that directory
    while(entry = readdir(dir))
    {
        if(checkDir(entry) == 1)
        {
        	//Actively updates the path by concatenating the old path with the name
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            printf("%s\n",entry->d_name);
            list_dir(path);
        }
        else if(checkFile(entry) == 1)
        {
            if(strcmp("core",entry->d_name)==0)
            {
            	printf("Removed %s", entry->d_name);
            	remove(entry->d_name);
            }
        }
    }
    closedir(dir);
}
//Name: checkDir
//Parameters: a pointer ot a dirent (has information about an entry in a DIR)
//Description: checks to see if an entry is a directory or not
//Return: 1 if a directory, 0 if not
int checkDir(struct dirent * entry)
{
    struct stat statS;
    stat(entry->d_name,&statS);
    if(S_ISDIR(statS.st_mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//Name: checkFile
//Parameters: a pointer to a dirent (has information about an entry in a DIR)
//Description: checks to see if an entry is a regular file or not
//Return: 1 if regular file, 0 if not
int checkFile(struct dirent * entry)
{
    struct stat statS;
    stat(entry->d_name,&statS);
    if(S_ISREG(statS.st_mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}