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

char choice[32];
void ParseFileInDir(DIR *dir, int counter)
{
  int k;
  struct dirent *d;
 char extension[2]=".c";
  
  while ((d = readdir(dir))!=NULL)
  {
    if(strcmp(d->d_name,extension))
        counter++;
    closedir(dir);
  }
  
}
void regFileMenu(struct stat sb, char *path)
{
        
    printf("Regular File: %s \n", path);
    printf("Menu for regular file \n");
    printf("1. Read -n \n");
    printf("2. Size -d \n");
    printf("3. Number of hard links -h \n");
    printf("4. Time of last modification -m \n");
    printf("5. Access rights -a \n");
    printf("6. Create a symbolic link give:link name -l \n");
                  
           
}
void symFileMenu(struct stat sb, char *path)
{
        printf("Symbolic Link: %s \n", path);
        printf("Menu for symbolic link \n");
        printf("1. Link name -n \n");
        printf("2. Delete link -l \n");
        printf("3. Size of the link -d \n");
        printf("4. Size of the target -t \n");
        printf("5. Access rights -a \n");
          
}
void DirFileMenu(DIR *dir,char *path)
{
    printf("Directory: %s \n",path);
    printf("Menu for directory link \n");
    printf("1. Directory name : -n \n");
    printf("2. Size of the directory -d \n");
    printf("3. Access rights -a \n");
    printf("4. Total number of files with .c extension \n");
}
int main(int argc, char *argv[])
{
    struct stat sb;
    DIR *dir;
    
    char path[256];
    int nrCFiles=0;


           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (lstat(argv[1], &sb) == -1) {
               perror("lstat");
               
               exit(EXIT_FAILURE);
           }
            strcpy(path,argv[1]);
           if (S_ISREG(sb.st_mode))
           {
            
            regFileMenu(sb,path);
           }
           if(S_ISLNK(sb.st_mode))
            {
            symFileMenu(sb,path);
            }
           if(S_ISDIR(sb.st_mode))
            {
                dir = opendir(path);
                if(dir == NULL)
                    {
                        printf("Error could not open the directory");
                    }
                 DirFileMenu(dir,path);
            }
          

            scanf("%s",choice); 
            system("clear");
           
            printf("Your choice %s  \n",choice);
           
           int i = 0;
           while(choice[i] != '\0') 
           {
            
                if(choice[0] == '-')
                {
                    if ( i!=0  && S_ISREG(sb.st_mode))
                    {
                        switch(tolower(choice[i]))
                        {
                            case 'n':
                                printf("File name : %s \n",argv[1]);
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
                                printf("Access rights %d %d\n",sb.st_uid,sb.st_gid);
                                break;
                            case 'l':
                                printf("Create a symbolic link give:link name -l \n");
                                printf("Introduce the name of the new link\n");
                                char link[100];
                                scanf("%s",link);
                                symlink(argv[1],link);
                                break;
                            default:
                               printf("Invalid Operation %c\n",choice[i]);
                                
                               
                               break;
                               
                        }
                    }
                    if(i!=0 && S_ISLNK(sb.st_mode))
                    {
                        switch(tolower(choice[i]))
                        {
                            case 'n':
                                printf("Link name : %s \n",argv[1]);
                                break;
                            case 'l':
                                unlink(argv[1]);   
                                printf("Deleting the link : %s\n",argv[1]);    
                                break;
                            case 'd':
                                printf("Size of the link %ld \n",sb.st_size);
                                break;
                            case 't':
                                struct stat sbNew;
                                stat(argv[1], &sbNew);
                                printf("Size of the target %ld\n",sbNew.st_size);
                                break;
                            case 'a':
                                printf("Access rights   %d %d\n",sb.st_uid,sb.st_gid);
                                break;
                            
                            default:
                              printf("Invalid Operation %c\n",choice[i]); 
                              break;
                        }
                    }
                    if(i!= 0 && S_ISDIR(sb.st_mode))
                    {
                        switch (tolower(choice[i]))
                        {
                        case 'n':
                            printf("Directory name : %s \n",path);
                            break;
                        case 'd':
                            struct stat sbNew2;
                            stat(path,&sbNew2);
                            printf("Size of the directory: %ld \n",sbNew2.st_size);
                            break;
                        case 'a':
                            printf("Access rights %d \n",sb.st_uid);
                            break;
                        case 'c':
                        printf("Total number of files with .c extension\n");
                            ParseFileInDir(dir,nrCFiles);
                            
                            
                            break;
                        default:printf("Invalid option \n");
                            break;
                        }
                    }
                }
                else
                {
                    printf("Invalid Option %s\nTry -%s \n",choice,choice);

                    break;
                }
            i++;
           
           }


}
