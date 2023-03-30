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
char choice[32];
void regFileMenu(struct stat sb, char *argv[])
{
        
    printf("Regular File %s \n", argv[1]);
    printf("Menu for regular file \n");
    printf("1. Read -n \n");
    printf("2. Size -d \n");
    printf("3. Number of hard links -h \n");
    printf("4. Time of last modification -m \n");
    printf("5. Access rights -a \n");
    printf("6. Create a symbolic link give:link name -l \n");
                  
           
}
void symFileMenu(struct stat sb, char *argv[])
{
        printf("Symbolic Link %s \n", argv[1]);
        printf("Menu for symbolic link \n");
        printf("1. Link name -n \n");
        printf("2. Delete link -l \n");
        printf("3. Size of the link -d \n");
        printf("4. Size of the target -t \n");
        printf("5. Access rights -a \n");
          
}
int main(int argc, char *argv[])
{
    struct stat sb;
    
           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }
        
           if (lstat(argv[1], &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
           }
           if (S_ISREG(sb.st_mode))
           {
            regFileMenu(sb,&argv[1]);
           }
           if(S_ISLNK(sb.st_mode))
            {
            symFileMenu(sb,&argv[1]);
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
                }
                else
                {
                    printf("Invalid Option %s\nTry -%s \n",choice,choice);

                    break;
                }
            i++;
           
           }


}
