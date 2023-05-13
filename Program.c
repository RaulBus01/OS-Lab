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
                printf(RED "Error creating the link\n" RESET);
                
            }
            else
            {
                
                printf(GRN "Link created successfully\n" RESET);

            }

            break;
        default:
           printf("Invalid Operation %c\n",options[i]);
            printf(RESET);
        
            break;
            
                
        }
    }
}
void linkFile(struct stat sb, char *fileName,char *options)
{
    int deleteLink =0;

    for(int i=1;i<strlen(options)  ;i++)
    {
        if(deleteLink == 1)
        {
            printf(RED "Link was deleted\n");
            break;
        }
        printf(GRN);
        switch(options[i])
        {

            case 'n':
                printf("Link name : %s \n",fileName);
                 printf(RESET);
                break;
            case 'l':
                unlink(fileName);   
                deleteLink =1;
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
                printf(RED "Invalid Operation %c\n",options[i]); 
                 printf(RESET);
                break;
        }
    
    }
}
void directoryFile(char *options,struct stat sb,char *fileName, DIR *dir)
{
   
    int nrCFiles=0;


    for(int i=1;i<strlen(options);i++)
    {

    
        printf(GRN);
            switch (options[i])
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
                printf(RED "!Invalid option %c\n",options[i]);
                 printf(RESET);
                break;
            }
    }
}
int checkOptions(struct stat sb, char *options)
{
    int i=1;
    if(options[0] != '-')
    {
        printf(RED"Invalid option\n");
        printf(RED"Options must start with -\n"RESET);
        return 0;
    }
    if(S_ISREG(sb.st_mode))
    {
        while(options[i] != '\0')
        {
            if(options[i] != 'n' && options[i] != 'd' && options[i] != 'h' && options[i] != 'm' && options[i] != 'a' && options[i] != 'l')
            {
                printf(RED"Invalid option %c\n",options[i]);
                return 0;
            }
            i++;
        }
    }
    if(S_ISLNK(sb.st_mode))
    {
        while(options[i] != '\0')
        {
            if(options[i] != 'n' && options[i] != 'l' && options[i] != 'd' && options[i] != 't' && options[i] != 'a')
            {
                printf(RED"Invalid option %c\n",options[i]);
                return 0;
            }
            i++;
        }
    }
    if(S_ISDIR(sb.st_mode))
    {
        while(options[i] != '\0')
        {
            if(options[i] != 'n' && options[i] != 'd' && options[i] != 'a' && options[i] != 'c')
            {
                printf(RED"Invalid option %c\n",options[i]);
                return 0;
            }
            i++;
        }
    }
    return 1;
    
}
void handleOptions(struct stat sb,char *fileName)
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
    sleep(1);
    
    printf(BLU"Enter your options : "RESET);
    
    if(scanf("%s",options) <0)
    {
        printf(RED"Error reading the options\n");
        printf(RESET);
        exit(EXIT_FAILURE);
    }
    system("clear");

    printf(GRN"\nOptions : %s\n",options);
    printf(RESET);

    if(checkOptions(sb,options) == 0)
    {
        handleOptions(sb,fileName);
        strcpy(options,"");
    }
    else
    {
        if(S_ISREG(sb.st_mode))
        {
            regularFile(sb,fileName,options);
        }
        if(S_ISLNK(sb.st_mode))
        {
           linkFile(sb,fileName,options);
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

            directoryFile(options,sb,fileName,dir);
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

void menuFunction(struct stat sb,char *path)
{
   
   DIR *dir;
   regex_t extensionC;
  
   int pfd[2];
   int isCFile = 0;
	
   

        if (S_ISREG(sb.st_mode))
        {
            
           //Compile the regular expression for .c files
           if(regcomp(&extensionC,".c$",REG_EXTENDED !=0))
           {
            printf(RED"Error compiling .c regular expression \n"RESET);
            exit(EXIT_FAILURE);
           }

            //Regex to find the regular files that have .c extension
           if(regexec(&extensionC,path, 0, NULL, 0) == 0)
           {
         
	      
            if(pipe(pfd)<0)
            {
                printf(RED"Pipe creation failed \n");
                exit(1);
            }
              
            //Create a child process
              pid_t cpid = fork();// Pentru fork 2 e nevoide ce close la pipe
            
           
               //Check if the fork function was called successfully
              if(cpid == -1)
                {
                    perror(RED "Fork failure \n" RESET);
                    exit(EXIT_FAILURE);
                }
                //Increase the counter of children
                 pidCounter++;
                 //Flag for checking if the file is a C file and to verify in the parent process
                   isCFile = 1;
              if(cpid == 0)
              {
                //close the reading end of the pipe
                close(pfd[0]);
                
                //Redirect the information from STDOUT to pipe
                int newfd = dup2(pfd[1],1);
                // Check if the dup2 call is succesful
                if(newfd < 0)
                {
                    printf(RED"Error dup2 \n");
                    exit(1);
                }
               
                //Call of  the script which write in the fileout.txt the errors and warnings from path
                execlp("bash","bash","script.sh",path,"fileout.txt",NULL);

                // Code executed if execlp is wrong
                printf(RED"!GOOOD" RESET);
                exit(1);
                    
              }
               if(isCFile == 1)
            {
                //Close the writing end of the pipe
                if(close(pfd[1]) < 0)
                {
                    printf(RED"Error closing the pipe \n"RESET);
                    exit(1);
                }
                //Buffer for the data from the reading end of the pipe
                char buff[512],*ptr,ch[1];
                //Number of errors and warnings to be extracted from the buffer
                int nrErrors,nrWarnings;
                //Assign the number of readed bytes
                int nrBytesRead = 0;
                while(read(pfd[0],ch,1) > 0)
                {
                    buff[nrBytesRead] = ch[0];
                    nrBytesRead++;
                }
                if(nrBytesRead > 0)
                {
                    //Termiante the string at the numbero of readed bytes
                    buff[nrBytesRead] = '\0';
                    
                    //Extract the numbers for erros and warnings in base 10
                    nrErrors = strtol(buff, &ptr, 10);
                    nrWarnings = strtol(ptr, NULL, 10);
                    //Get the score using the function
                    double score = getScore(nrWarnings,nrErrors);
                    //Print the Number of Warnings and Errors and the score
                    FILE *f = fopen("grades.txt","a");
                    if(f == NULL)
                    {
                        printf(RED"Error opening the file grades.txt \n"RESET);
                        exit(1);
                    }
                    fprintf(f,"In file: %s found  %d warnings %d errors .The score is %.2f \n",path,nrWarnings,nrErrors,score);
                   
                    if(fclose(f) != 0)
                    {
                        printf(RED"Error closing the file grades.txt \n"RESET);
                        exit(1);
                    }

                
                }
            
          
                //Close the reading end of the pipe
                if(close(pfd[0]) < 0)
                {
                    printf(RED"Error closing the pipe \n"RESET);
                    exit(1);
                
                }
            }
                

           }
           else
           {
                pid_t cpidReg = fork();
                if(cpidReg == -1)
                {
                    printf(RED "Fork failure \n" RESET);
                    exit(EXIT_FAILURE);     
                }
                pidCounter++;
                if(cpidReg == 0)
                {
                    //Child code 
                    //Change the regular file access rights
                    execlp("bash","bash","scriptLines.sh",path,NULL);
                    printf(RED"Not GOOOD" RESET);
                    exit(1);
                }
           }
           
           
            
        }
 

        if(S_ISLNK(sb.st_mode))
        {
            
          
            pid_t cpidLnk = fork();
               if(cpidLnk == -1)
            {
                printf(RED "Fork failure \n" RESET);
                exit(EXIT_FAILURE);     
            }
            pidCounter++;
            if(cpidLnk == 0)
            {
                //Child code 
                
                //Change the target file access rights
                execlp("chmod","chmod","-v","760",path,NULL);
                 printf(RED"Not GOOOD" RESET);
                exit(1);
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
                    printf(RED "Not GOOOD" RESET);
                    exit(1);
                   
                }
                  

               
              
        }
        // child to handle the options
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
            handleOptions(sb,path);
           
            exit(0);
        }
        
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
                
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(status)) 
            {
                printf(GRN"The process with PID  <%d> has ended with the  status=%d\n",w, WEXITSTATUS(status));
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
            printf(RED "File '%s' not exist \n",argv[i]);
            printf(RESET);
            
            exit(EXIT_FAILURE);
        }
        // Copy the path to the path variable
        strcpy(path,argv[i]);
         
        // Call the menu function
    
        menuFunction(sb,path);
       
    }


}