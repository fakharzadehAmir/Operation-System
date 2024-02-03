#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
char *FileName;
char **words;
char *path;
FILE *file;
int totalNum;
void countOcc(int queryId, char* word) {
   file = fopen(path, "r");
   int numberOfWord = 0;
   char *line;
   line = malloc(sizeof(char) * (int)5e6);
//  read lines for reading completely I had to allocate a huge size for each line.
   while (fgets(line, sizeof(char) * (int)5e6, file))
   {
//      Check if in that line the word we're looking for exists.
       if (strstr(line, words[queryId - 1]) != NULL)
       {
           numberOfWord++;
           totalNum++;
       }
   }
   fclose(file);
   free(line);
   printf("query%d(%s): %d\n", queryId, word, numberOfWord);
}

int main() {
   FileName = malloc(sizeof(char)*100);
   scanf("search %s", FileName);
   words = malloc(sizeof(char*)*100);
   int size = 0;
   while(1) {
       words[size] = malloc(sizeof(char)*100);
       scanf("%s",words[size]);
       if(strcmp(words[size], ".") == 0) {
           break;
       }
       size++;
   }
   path = malloc(sizeof(char)*256);
//  This is the path of cmake-build-debug; So I must change the given path.
//    cmake-build-debug/Assignment
   snprintf(path, 256, "../HW2/Q2/%s", FileName);
   for(int i = 0; i < size; i++) {
       pid_t pid = vfork();
       if (pid == 0) {
           countOcc(i+1, words[i]);
           exit(0);
       }
       else {
           wait(0);
       }
   }
   printf("Total occurrences: %d\n", totalNum);
   free(path);
   for(int i = 0; i < size; i++){
       free(words[i]);
   }
   free(words);
   free(FileName);
}