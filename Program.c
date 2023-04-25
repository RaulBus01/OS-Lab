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
#include <sys/wait.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

char choice[32];
char path[256];

int nrFiles = 0;
int optionForMenu = 0;
int pidCounter = 0;

int ParseFileInDir(DIR *dir)
{
  int counter = 0;
  struct dirent *entry;
  struct stat file;
  regex_t extension;

    if(regcomp(&extension, ".c$", REG_EXTENDED))
    {
        printf( RED "RError compiling the regular expression\n" RESET);
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
        
        if(lstat(auxpath,&file) == -1)
        {
            printf(RED "Error could not get the file stats" RESET);
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
        
    printf(GRN "\n Regular File:");
    printf(BLU "%s \n", path);
    printf("Menu for regular file \n");
    printf(YEL"1. Read: -n \n");
    printf("2. Size: -d \n");
    printf("3. Number of hard links: -h \n");
    printf("4. Time of last modification: -m \n");
    printf("5. Access rights: -a \n");
    printf("6. Create a symbolic link -l \n" RESET);
                  
           
}
void symFileMenu(struct stat sb, char *path)
{
        printf(GRN "\nSymbolic Link:"); 
        printf(BLU"%s\n",path);
        printf("Menu for symbolic link \n");
        printf(YEL "1. Link name: -n \n");
        printf("2. Delete link: -l \n");
        printf("3. Size of the link: -d \n");
        printf("4. Size of the target: -t \n");
        printf("5. Access rights: -a \n" RESET);
          
}
void dirFileMenu(DIR *dir,char *path)
{
    printf(GRN"\nDirectory:");
    printf(BLU"%s\n",path);
    printf(YEL"Menu for directory file \n");
    printf("1. Directory name : -n \n");
    printf("2. Size of the directory: -d \n");
    printf("3. Access rights: -a \n");
    printf("4. Total number of files with .c extension: -c \n" RESET);
}
void accessRights(struct stat sb)
{
    printf(RED "\nAccess rights for owner: \n \n");
    if(sb.st_mode & S_IRUSR)
        printf(CYN "->Read permission for owner\n");
    if(sb.st_mode & S_IWUSR)
        printf(MAG "->Write permission for owner\n");
    if(sb.st_mode & S_IXUSR)
        printf(YEL"->Execute permission for owner\n");
    printf(GRN"\nAccess rights for group: \n \n");
    if(sb.st_mode & S_IRGRP)
        printf(CYN"->Read permission for group\n");
    if(sb.st_mode & S_IWGRP)
        printf(MAG"->Write permission for group\n");
    if(sb.st_mode & S_IXGRP)
        printf(YEL"->Execute permission for group\n");
    printf(BLU"\nAccess rights for others: \n \n");
    if(sb.st_mode & S_IROTH)
        printf(CYN"->Read permission for others\n");
    if(sb.st_mode & S_IWOTH)
        printf(MAG"->Write permission for others\n");
    if(sb.st_mode & S_IXOTH)
        printf(YEL"->Execute permission for others\n");
    
    printf(RESET"\n");

    
}
void regularFile(int i, struct stat sb, char *fileName)
{
    if ( i!=0  && S_ISREG(sb.st_mode))
    {

        printf(GRN);
        switch(choice[i])
        {
            case 'n':
                printf("File name : %s \n",fileName);
                printf(RESET);
                break;
            case 'd':
                printf("Size  %ld \n",sb.st_size);
                 printf(RESET);
                break;
            case 'h':
                
                printf( "Number of hard links   %ld\n",sb.st_nlink);
                printf(RESET);
                break;
            case 'm':
                printf("Time of last modification %s",ctime(&sb.st_atime));
                printf(RESET);
                break;
            case 'a':
                printf(RESET);
                accessRights(sb);
                break;
            case 'l':
                printf("Introduce the name of the new link :\n");
                char link[100];
                scanf("%s",link);
                if(symlink(fileName,link) == -1)
                {
                    printf(RED "Error creating the link" RESET);
                    exit(EXIT_FAILURE);
                }
                printf(GRN "Link created successfully\n" RESET);

                break;
            default:
                printf(RED "Invalid Operation %c\n",choice[i]);
                printf(RESET);
               
                break;
                
        }
    }
}
void linkFile(int i, struct stat sb, char *fileName)
{
    if(i!=0 && S_ISLNK(sb.st_mode))
    {
    
        printf(GRN);
        switch(choice[i])
        {

            case 'n':
                printf("Link name : %s \n",fileName);
                 printf(RESET);
                break;
            case 'l':
                unlink(fileName);   
                printf("Deleting the link : %s\n",fileName);
                 printf(RESET); 
                break;
            case 'd':
                printf("Size of the link %ld \n",sb.st_size);
                 printf(RESET);
                break;
            case 't':
                struct stat sbNew;
                stat(fileName, &sbNew);
                printf("Size of the target %ld\n",sbNew.st_size);
                 printf(RESET);
                break;
            case 'a':
                
                accessRights(sb);
                 printf(RESET);
                break;
            
            default:
                printf(RED "Invalid Operation %c\n",choice[i]); 
                 printf(RESET);
                break;
        }
    }
}
void directoryFile(int i,struct stat sb,char *fileName, DIR *dir)
{
   
    int nrCFiles=0;


    
     if(i!= 0 && S_ISDIR(sb.st_mode))
        {printf(GRN);
            switch (choice[i])
            {
                
            case 'n':
                printf("Directory name : %s \n",fileName);
                 printf(RESET);
                break;
            case 'd':
                struct stat sbNew2;
                stat(fileName,&sbNew2);
                printf("Size of the directory: %ld \n",sbNew2.st_size);
                 printf(RESET);
                break;
            case 'a':
               
               accessRights(sb);
               printf(RESET);
                break;
            case 'c':
            strcat(path,"/");
            nrCFiles = ParseFileInDir(dir);
            printf("Total number of files with .c extension : %d \n",nrCFiles);
            printf(RESET);
                break;
            default:
                printf(RED "Invalid option %c\n",choice[i]);
                 printf(RESET);
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
            printf(RED "\nTry -%s \n \n",choice);
            printf(RESET);
            return;
        }
        //Check if the option is valid if not print the specific option that is invalid
    while(choice[charIndex]!='\0')
        {
            if(strchr(validCommands,choice[charIndex]) == NULL)
            {
                printf(RED "Invalid option: %c \n",choice[charIndex]);
                printf(RESET);
            }
            charIndex++;
        }
        printf("\n");
}
void choiceFunction(struct stat sb,char* path)
{
    if(optionForMenu == 1)
    {
        regFileMenu(sb,path);
    }
    if(optionForMenu == 2)
    {
        symFileMenu(sb,path);
    }
    if(optionForMenu == 3)
    {

        DIR *dir;
        //Open the directory 
          dir = opendir(path);
        //Check if the directory was opened correctly
            if(dir == NULL)
        {
            printf(RED "Error could not open the directory" RESET);
        }

        dirFileMenu(dir,path);

        closedir(dir);
       
    }
    // Get the choice from the user 
        printf("\n"BLU);
        printf("Enter your choice:" RESET);
        sleep(1);
        scanf("%s",choice);
        
        // Clear the screen for a better user experience 
        system("clear");
        

        printf(BLU"Your choice %s  \n",choice);
        printf(RESET);
}
void menuFunction(struct stat sb,char *path)
{
   
   DIR *dir;
   regex_t extension,extensionC;
   char *validCommands;
    //Option for menu: 1 for Regular File | 2 for Symbolic Link File | 3 for Directory file

        if (S_ISREG(sb.st_mode))
        {
            //Setting the right option for regular File Menu
           optionForMenu = 1; 
           
           if(regcomp(&extensionC,".c$",REG_EXTENDED !=0))
           {
            printf(RED"Error compiling .c regular expression \n"RESET);
            exit(EXIT_FAILURE);
           }

            //Regex to find the regular files that have .c extension
           if(regexec(&extensionC,path, 0, NULL, 0) == 0)
           {
            //Create a child process
              pid_t cpid = fork();
               //Check if the fork function was called successfully
              if(cpid == -1)
                {
                    perror(RED "Fork failure \n" RESET);
                    exit(EXIT_FAILURE);
                }
                //Increase the counter of children
                 pidCounter++;
              if(cpid == 0)
              {
                
                //Call of  the script which write in the fileout.txt the errors and warnings from path
                execlp("bash","bash","script.sh",path,"fileout.txt",NULL);

                // Code executed if execlp is wrong
                printf(RED"!GOOOD" RESET);
                exit(1);
                    
              }
                

           }
           
            //Assign the valid commands for the regular file
            validCommands = "-ndhmal";

            //Compile the regular expression for the regular file
            if(regcomp(&extension,"^-[ndhmal]+$",REG_EXTENDED) != 0)
            {
                
                printf(RED "Error compiling the regular expression \n" RESET);
                exit(EXIT_FAILURE);
            }
            
        }
 

        if(S_ISLNK(sb.st_mode))
        {
            
             //Setting the right option for symbolic Link Menu
            optionForMenu = 2;
            //Assign the valid commands for the link
            validCommands = "-nldta";

            //Compile the regular expression for the link
            if(regcomp(&extension,"^-[nldta]+$",REG_EXTENDED) != 0)
            {
                
                printf(RED "Error compiling the regular expression" RESET);
                exit(EXIT_FAILURE);
            }
        
        }

        if(S_ISDIR(sb.st_mode))
        {
            //Open the directory 
                dir = opendir(path);
            //Check if the directory was opened correctly
                 if(dir == NULL)
                {
                    printf(RED "Error could not open the directory" RESET);
                }
                //Setting the right option for directory Menu
                optionForMenu = 3;
            
                pid_t cpidDir = fork();
                 //Check if the fork function was called successfully
                if(cpidDir == -1)
                {
                    printf(RED "Fork failure \n" RESET);
                    exit(EXIT_FAILURE);
                }
                //Obtain the name of the new file dirName_file.txt

                char newFileName[255]="";
                strcpy(newFileName,path);
                strcat(newFileName,"_file.txt");
                strcat(newFileName,"\0");
                //Increase the counter of children
                pidCounter++;
                if(cpidDir == 0)
                {
                    // 2nd child process 

                    //Call of  the script which create the file with the given name(newFileName)
                    execlp("bash","bash","scriptDirFile.sh",newFileName,NULL);
                   
                    // Code executed if execlp is wrong
                    printf(RED "!GOOOD" RESET);
                    exit(1);
                   
                }
                  

                //Assign the valid commands for the directory
                validCommands = "-ndac";
                //Compile the regular expression for the directory
                 if(regcomp(&extension,"^-[ndac]+$",REG_EXTENDED) != 0)
                {
                    
                    printf(RED "Error compiling the regular expression\n" RESET);
                    exit(EXIT_FAILURE);
                }
              
        }
        //2nc child process to handle the options
        pid_t pid = fork();
       //Check if the fork function was called successfully
        if(pid == -1)
        {
            printf(RED "Fork error \n" RESET);
            exit(EXIT_FAILURE);

        }
        //Increase the counter of children
        pidCounter++;
        if(pid == 0)
        {
            
            choiceFunction(sb,path);

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
            exit(1);
        }
        else
        {   
              
              
           //Wait for all childs
           for(int k = 0; k <pidCounter;k++)
           {
            
            int status;
            pid_t w;
        
            w=wait(&status);
            //Check the return value of wait function
            if (w == -1)
            {
                //printf("waitpid status %d",status);
                exit(EXIT_FAILURE);
            }
            
           }
           // Reset number of childs for each argument 
           pidCounter=0;
         
             
        }

    
}
int main(int argc, char *argv[])
{
    struct stat sb;
    nrFiles = argc;
    // Check if the user has entered the path
    if (nrFiles < 2) 
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    for(int i=1;i<nrFiles;i++)
    {
       
        // Check if the path is a valid file
        if (lstat(argv[i], &sb) == -1) 
        {
            printf(RED "File not exist \n" RESET);
            
            exit(EXIT_FAILURE);
        }
        // Copy the path to the path variable
        strcpy(path,argv[i]);
         
        // Call the menu function
    
        menuFunction(sb,path);
       
    }


}
