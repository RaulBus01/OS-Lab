#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <dirent.h>
#include <regex.h>



char choice[32];
char path[256];

int ParseFileInDir(DIR *dir)
{
  int counter = 0;
  struct dirent *entry;
  struct stat file;
  regex_t extension;
    if(regcomp(&extension, ".c$", REG_EXTENDED))
    {
        printf("Error compiling the regular expression\n");
        exit(EXIT_FAILURE);
    }
 
  while ((entry = readdir(dir))!=NULL)
  {
    
    
     
    char *name=entry->d_name;
    char  auxpath[4096];
    if(strlen(name) > 2)
    {
        
        strcpy(auxpath,path);
        strcat(auxpath,name);
        //printf("Name %s \n",auxpath);
        if(lstat(auxpath,&file) == -1)
        {
            printf("Error could not get the file stats");
            exit(EXIT_FAILURE);
        }
        if (S_ISREG(file.st_mode) && regexec(&extension, entry->d_name, 0, NULL, 0) == 0)
        {
            counter++;
        
        }
    }
   

    
        
    
  }
   closedir(dir);   
   return counter;  
  
}
void regFileMenu(struct stat sb, char *path)
{
        
    printf("Regular File: %s \n", path);
    printf("Menu for regular file \n");
    printf("1. Read: -n \n");
    printf("2. Size: -d \n");
    printf("3. Number of hard links: -h \n");
    printf("4. Time of last modification: -m \n");
    printf("5. Access rights: -a \n");
    printf("6. Create a symbolic link give:link name: -l \n");
                  
           
}
void symFileMenu(struct stat sb, char *path)
{
        printf("Symbolic Link: %s \n", path);
        printf("Menu for symbolic link \n");
        printf("1. Link name: -n \n");
        printf("2. Delete link: -l \n");
        printf("3. Size of the link: -d \n");
        printf("4. Size of the target: -t \n");
        printf("5. Access rights: -a \n");
          
}
void dirFileMenu(DIR *dir,char *path)
{
    printf("Directory: %s \n",path);
    printf("Menu for directory file \n");
    printf("1. Directory name : -n \n");
    printf("2. Size of the directory: -d \n");
    printf("3. Access rights: -a \n");
    printf("4. Total number of files with .c extension: -c \n");
}
void accessRights(struct stat sb)
{
    printf("Access rights for owner: \n \n");
    if(sb.st_mode & S_IRUSR)
        printf("Read permission for owner\n");
    if(sb.st_mode & S_IWUSR)
        printf("Write permission for owner\n");
    if(sb.st_mode & S_IXUSR)
        printf("Execute permission for owner\n");
    printf("\nAccess rights for group: \n \n");
    if(sb.st_mode & S_IRGRP)
        printf("Read permission for group\n");
    if(sb.st_mode & S_IWGRP)
        printf("Write permission for group\n");
    if(sb.st_mode & S_IXGRP)
        printf("Execute permission for group\n");
    printf("\nAccess rights for others: \n \n");
    if(sb.st_mode & S_IROTH)
        printf("Read permission for others\n");
    if(sb.st_mode & S_IWOTH)
        printf("Write permission for others\n");
    if(sb.st_mode & S_IXOTH)
        printf("Execute permission for others\n");
    
    printf("\n");

    
}
void regularFile(int i, struct stat sb, char *fileName)
{
    if ( i!=0  && S_ISREG(sb.st_mode))
    {
        switch(tolower(choice[i]))
        {
            case 'n':
                printf("File name : %s \n",fileName);
                break;
            case 'd':
                printf("Size  %ld \n",sb.st_size);
                break;
            case 'h':
                printf("Number of hard links   %ld\n",sb.st_nlink);
                break;
            case 'm':
                printf("Time of last modification %s",ctime(&sb.st_atime));
                break;
            case 'a':
                //printf("Access rights %d %d\n",sb.st_uid,sb.st_gid);
                accessRights(sb);
                break;
            case 'l':
                printf("Create a symbolic link give:link name -l \n");
                printf("Introduce the name of the new link\n");
                char link[100];
                scanf("%s",link);
                if(symlink(fileName,link) == -1)
                {
                    printf("Error creating the link");
                    exit(EXIT_FAILURE);
                }
                printf("Link created successfully\n");

                break;
            default:
                printf("Invalid Operation %c\n",choice[i]);
               
                break;
                
        }
    }
}
void linkFile(int i, struct stat sb, char *fileName)
{
    if(i!=0 && S_ISLNK(sb.st_mode))
    {
        switch(tolower(choice[i]))
        {
            case 'n':
                printf("Link name : %s \n",fileName);
                break;
            case 'l':
                unlink(fileName);   
                printf("Deleting the link : %s\n",fileName);    
                break;
            case 'd':
                printf("Size of the link %ld \n",sb.st_size);
                break;
            case 't':
                struct stat sbNew;
                stat(fileName, &sbNew);
                printf("Size of the target %ld\n",sbNew.st_size);
                break;
            case 'a':
                //printf("Access rights   %d %d\n",sb.st_uid,sb.st_gid);
                accessRights(sb);
                break;
            
            default:
                printf("Invalid Operation %c\n",choice[i]); 
                
                break;
        }
    }
}
void directoryFile(int i,struct stat sb,char *fileName, DIR *dir)
{
   
    int nrCFiles=0;
    //char *validCommands="ndac";

    
     if(i!= 0 && S_ISDIR(sb.st_mode))
        {
            switch (tolower(choice[i]))
            {
            case 'n':
                printf("Directory name : %s \n",fileName);
                break;
            case 'd':
                struct stat sbNew2;
                stat(fileName,&sbNew2);
                printf("Size of the directory: %ld \n",sbNew2.st_size);
                break;
            case 'a':
                //printf("Access rights %d \n",sb.st_uid);
               accessRights(sb);
                break;
            case 'c':
            strcat(path,"/");
            nrCFiles = ParseFileInDir(dir);
            printf("Total number of files with .c extension : %d \n",nrCFiles);
                
                break;
            default:
                printf("Invalid option %c\n",choice[i]);
                
                break;
            }
        }
}
void wrongOption(char *validCommands,char *choice)
{
     int charIndex = 0;
     //Check if the first character is a dash if not print the error message
     if(choice[0] !='-')
        {
            printf("Try -%s \n \n",choice);
            return;
        }
        //Check if the option is valid if not print the specific option that is invalid
    while(choice[charIndex]!='\0')
        {
            if(strchr(validCommands,choice[charIndex]) == NULL)
            {
                printf("Invalid option: %c \n",choice[charIndex]);
            }
            charIndex++;
        }
        printf("\n");
}
void menuFunction(struct stat sb,char *path)
{
    DIR *dir;
    regex_t extension;
    char *validCommands;
        if (S_ISREG(sb.st_mode))
        {
            //Call the regular file menu
            regFileMenu(sb,path);

            //Assign the valid commands for the regular file
            validCommands = "-ndhmal";

            //Compile the regular expression for the regular file
            if(regcomp(&extension,"^-[ndhmal]+$",REG_EXTENDED) != 0)
            {
                
                printf("Error compiling the regular expression");
            }
            
        }

        if(S_ISLNK(sb.st_mode))
        {
            //Call the link file menu
            symFileMenu(sb,path);

            //Assign the valid commands for the link
            validCommands = "-nldta";

            //Compile the regular expression for the link
            if(regcomp(&extension,"^-[nldta]+$",REG_EXTENDED) != 0)
            {
                
                printf("Error compiling the regular expression");
            }
        }

        if(S_ISDIR(sb.st_mode))
        {
            //Open the directory 
                dir = opendir(path);
            //Check if the directory was opened correctly
            if(dir == NULL)
                {
                    printf("Error could not open the directory");
                }
                //Call the directory file menu
                dirFileMenu(dir,path);

                //Assign the valid commands for the directory
                validCommands = "-ndac";
                //Compile the regular expression for the directory
            if(regcomp(&extension,"^-[ndac]+$",REG_EXTENDED) != 0)
                {
                    
                    printf("Error compiling the regular expression");
                }
                  
               
              
                
        }
        // Get the choice from the user 
        printf("Enter your choice: ");
        scanf("%s",choice);
        // Clear the screen for a better user experience 
        system("clear");
        

        printf("Your choice %s  \n",choice);
        // Check if the choice is valid
        if(regexec(&extension,choice,0,NULL,0) != 0)
        {
            // Call the function that will print the invalid options
           wrongOption(validCommands,choice);
            // Recursive call to the menu function
            menuFunction(sb,path);
            // Reset the choice to empty string because the recurisve calls will print the choice again
            strcpy(choice,"");
        }
        int i = 0;
        while(choice[i] != '\0') 
        {
            //Apply the functions that were choosen
            
            // Regular File Functions
            regularFile(i,sb,path);

            // Link File Function
            linkFile(i,sb,path);

            // Directory File Function

            directoryFile(i,sb,path,dir);
    
            i++;
    
        }
}
int main(int argc, char *argv[])
{
    struct stat sb;
    // Check if the user has entered the path
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
        // Check if the path is a valid file
    if (lstat(argv[1], &sb) == -1) 
    {
        perror("lstat");
        
        exit(EXIT_FAILURE);
    }
    // Copy the path to the path variable
    strcpy(path,argv[1]);
    // Call the menu function
    menuFunction(sb,path);
    


}
