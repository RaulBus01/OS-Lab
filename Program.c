#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <sys/sysmacros.h>

int main(int argc, char *argv[])
{
    struct stat sb;
    char choice[7];
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
               printf("regular file %s \n", argv[1]);
               printf("Menu for regular file \n");
               printf("1. Read -n \n");
               printf("2. Size -d \n");
               printf("3. Number of hard links -h \n");
               printf("4. Time of last modification -m \n");
               printf("5. Access rights -a \n");
               printf("6. Create a symbolic link give:link name -l \n");
               
           }
           if(S_ISLNK(sb.st_mode))
           {
               printf("symbolic link %s \n", argv[1]);
               printf("Menu for symbolic link \n");
               printf("1. Link name -n \n");
               printf("2. Delete link -l \n");
               printf("3. Size of the link -d \n");
               printf("4. Size of the target -t \n");
               printf("5. Access rights -a \n");
               
           }
           scanf("%s", choice);
           int i = 0;
           while(choice[i] != '\0')
           {
                if(choice[0] == '-')
                {
                    if ( i!=0 && strchr("nldhma", choice[i]))
                    {
                        switch(choice[i])
                        {
                            case 'n':
                                printf("File name : %s \n",argv[1]);
                                break;
                            case 'd':
                                printf("Size  %ld \n",sb.st_size);
                                break;
                            case 'h':
                                printf("Number of hard links -h  %ld\n",sb.st_nlink);
                                break;
                            case 'm':
                                printf("Time of last modification -m  %s \n",ctime(&sb.st_atime));
                                break;
                            case 'a':
                                printf("Access rights -a  %d %d\n",sb.st_uid,sb.st_gid);
                                break;
                            case 'l':
                                printf("Create a symbolic link give:link name -l \n");
                                break;
                            default:
                                printf("Invalid option \n");
                                break;
                        }
                    }
                }
                else 
                {
                    printf("Not valid option");
                }
            i++;
           }


}
