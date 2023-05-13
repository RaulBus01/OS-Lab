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

int pidCounter = 0;

int ParseFileInDir(DIR *dir)
{
  int counter = 0;
  struct dirent *entry;
  struct stat file;
  regex_t extension;

    if(regcomp(&extension, ".c$", REG_EXTENDED))
    {
        printf( RED "Error compiling the regular expression\n" RESET);
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
void regularFile(struct stat sb, char *fileName,char *options)
{

  
    for(int i=1;i<strlen(options);i++)
    {
                printf(GRN);
                switch(options[i])
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
                        if(scanf("%s",link) < 0 )
                        {
                            printf(RED"Error scanf \n");
                            printf(RESET);
                        }
                        
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


double getScore(int nrWarnings, int nrErrors)
{   int score = 0;
    if(nrWarnings == 0 && nrWarnings == 0)
    {
        score = 10;
        
    }
    if(nrErrors >= 1)
    {
        score = 1;
    }
    if(nrErrors == 0 && nrWarnings > 10)
    {
        score = 2;
    }
     if(nrErrors == 0 && nrWarnings  <=10)
    {
        score = 2 + 8 * (10-nrWarnings) / 10;
    }
    return score;
}
void printMenu(struct stat sb,char *fileName)
{
    char options[100];
    if(S_ISREG(sb.st_mode))
    {
        regFileMenu(sb,fileName);
    }
   
    if(S_ISLNK(sb.st_mode))
    {
        symFileMenu(sb,fileName);
    }
     if(S_ISDIR(sb.st_mode))
    {
        DIR *dir;
        if((dir = opendir(path)) == NULL)
        {
            printf(RED "Error opening the directory\n");
            printf(RESET);
            exit(EXIT_FAILURE);
        }

        dirFileMenu(dir,fileName);
    }
    printf("Enter your choice : \n");
    scanf("%s",options);
    printf("Choice : %s\n",options);

    if(checkOptions(sb,options) == 0)
    {
        printMenu(sb,fileName);
        strcpy(options,"");
    }
    else
    {
        regularFile(sb,fileName,options);
    }



}
int checkOptions(struct stat sb, char *options)
{
    int i=1;
    if(options[0] != '-')
    {
        printf("Invalid option\n");
        printf("Options must start with -\n");
        return 0;
    }
    while(options[i] != '\0')
    {
        if(options[i] != 'n' && options[i] != 'd' && options[i] != 'h' && options[i] != 'm' && options[i] != 'a' && options[i] != 'l' && options[i] != 't' && options[i] != 'c')
        {
            printf("Invalid option %c\n",options[i]);
            return 0;
        }
        i++;
    }
    return 1;
    
}
void menuFunction(struct stat sb,char *path)
{
    pid_t pidOptions;
    int pfd[2];
    

    pidOptions = fork();
    if(pidOptions == -1)
    {
        printf(RED "Error fork\n");
        printf(RESET);
        exit(EXIT_FAILURE);
    }  
    pidCounter++;
    if(pidOptions == 0)
    {
        printMenu(sb,path);
    
    }
    else
    {
        if(pipe(pfd) == -1)
        {
            printf(RED "Error pipe\n");
            printf(RESET);
            exit(EXIT_FAILURE);
        }
        pid_t pidScore;
        pidScore = fork();
        if(pidScore == -1)
        {
            printf(RED "Error fork\n");
            printf(RESET);
            exit(EXIT_FAILURE);
        }
        pidCounter++;
        if(pidScore == 0)
        {
            close(pfd[0]);
            int dupResult = dup2(pfd[1],1);
            if(dupResult == -1)
            {
                printf(RED "Error dup2\n");
                printf(RESET);
                exit(EXIT_FAILURE);
            }
            execlp("bash","bash","script.sh",path,"fileout.txt",NULL);
            printf(RED "Error execlp\n");
            printf(RESET);

        }
       
    }
    
        
            if(close(pfd[1]) == -1)
            {
                printf(RED "Error close\n");
                printf(RESET);
                exit(EXIT_FAILURE);
            }
           
            char buff[512],*ptr,*ch;
            while(read(pfd[0],ch,1) > 0)
            {
                strncat(buff,ch,1);
            }
             close(pfd[0]);
             printf("buff : %s\n",buff);
            int nrErrors,nrWarnings;
            nrErrors = strtol(buff, &ptr, 10);
            nrWarnings = strtol(ptr, NULL, 10);
            double score = getScore(nrWarnings,nrErrors);
           FILE *f = fopen("grades.txt","a");
                    if(f == NULL)
                    {
                        printf(RED"Error opening grades.txt \n"RESET);
                        exit(EXIT_FAILURE);
                    }
                    fprintf(f,"Warnings -> %d Errors -> %d \n",nrWarnings,nrErrors);
                    fprintf(f,"Score -> %.2f \n",score);
                    fclose(f);
                   
                 //Close the reading end of the pipe
           
          
        
   
   

            // //If the file is a C file then
           
            //     //Close the writing end of the pipe
            //     close(pfd[1]);
            //     //Buffer for the data from the reading end of the pipe
            //     char buff[512],*ptr,*ch;
            //     //Number of errors and warnings to be extracted from the buffer
            //     int nrErrors,nrWarnings;
            //     //Assign the number of readed bytes
            //     while( read(pfd[0],ch,1) == 1)
            //     {
            //         strncat(buff,ch,1);
            //     }
                
            //         //Extract the numbers for erros and warnings in base 10
            //         nrErrors = strtol(buff, &ptr, 10);
            //         nrWarnings = strtol(ptr, NULL, 10);
            //         //Get the score using the function
            //         double score = getScore(nrWarnings,nrErrors);
            //         //Print the Number of Warnings and Errors and the score
            //         FILE *f = fopen("grades.txt","a");
            //         if(f == NULL)
            //         {
            //             printf(RED"Error opening grades.txt \n"RESET);
            //             exit(EXIT_FAILURE);
            //         }
            //         fprintf(f,"Warnings -> %d Errors -> %d \n",nrWarnings,nrErrors);
            //         fprintf(f,"Score -> %.2f \n",score);
            //         fclose(f);
                   
            //      //Close the reading end of the pipe
            // close(pfd[0]);
                
            
        
           
            
            
           
            
         
            
           
           

           
           //Wait for all childs
           for(int k = 0; k <pidCounter;k++)
           {
            
            int status;
            pid_t w;
           //
            w=wait(&status);
            //Check the return value of wait function
            if (w == -1)
            {
                //printf("waitpid status %d",status);
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(status)) 
            {
                printf(GRN"Exited  %d with the  status=%d\n",w, WEXITSTATUS(status));
                printf(RESET);
             } 
             else if (WIFSIGNALED(status)) 
             {
                 printf(RED"Killed by signal %d\n", WTERMSIG(status));

            }
             else if (WIFSTOPPED(status)) 
             {
                printf(BLU"Stopped by signal %d\n", WSTOPSIG(status));
                printf(RESET);

             } 
             else if (WIFCONTINUED(status)) 
             {
                 printf(BLU"Continued\n");
                 printf(RESET);
            }
            
           }
           // Reset number of childs for each argument 
           pidCounter=0;

             
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