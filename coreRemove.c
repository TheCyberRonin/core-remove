/*********************************************************************************
 * Ruben Swyers                                                                  *
 * Accepts a file or directory as a command line argument					     *
 * it will search throught it and list directories							     *
 * and cores in them along with some information about them.					 *
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
void list_dir(char * name,int level);

int main(int argc, char *argv[])
{
	//Usage clause:
	//Expecting one command line argument, a directory name
	if(argc != 2)
	{
		printf("usage: %s <directory>\n",argv[0]);
	}

	//variables
	struct stat statS;
    //recursively searches for a core file, prints its location and removes it
	stat(argv[1],&statS);
	if(S_ISDIR(statS.st_mode))
	{
        //recursively gets and prints the information out of the contents of the directory that have the name of core
		list_dir(argv[1],0);
	}
	else
	{
		printf("The command line argument was not a directory\n");
	}
	printf("\n");
	return 0;
}
//Name: list_dir
//Parameters: name of the directory and the level (int)
//Description: prints out the contents of a directory recursively that is removed(core files)
void list_dir(char * name, int level)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statS;
    int offset = level*2;
    int error;
    int flag = 0;
    //Opens the directory
    if (!(dir = opendir(name)))
    {
        return;
    }
    //make the current directory the one that is passed
    chdir(name);
    //if its a directory
    //it calls list_dir for that directory
    while((entry = readdir(dir)) !=NULL)
    {
      	if(lstat(entry->d_name,&statS)==-1)
		{
	  	perror("Stat");
		}
        //checks to see if the entry is a directory
        if(S_ISDIR(statS.st_mode))
        {
        	//skips over "." and ".."
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            printf("%*s%s/\n",offset,"",entry->d_name);
            list_dir(entry->d_name,level+1);
        }
        //if it's not a directory its a file,looks to see if it is a core file, prints the location and removes it
        else
        {	
        	if(strcmp("core",entry->d_name) == 0)
        	{
        		printf("%*sremoved: %s%20d bytes\n",offset,"",entry->d_name,(int)statS.st_size);
        		unlink(entry->d_name);
        		flag = 1;
        	}
            
        }
    }
    if(flag == 0)
    {
    	printf("%*sNo cores found in this directory\n",offset,"");
    }
    //change the directory back to its parent
    chdir("..");
    closedir(dir);
}
