#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.c"
// #include "graph.h"


char* replace(char *arr , int len, int index,char *str){
    int i = index;
    int j = 0;
    for (int n= 0; n < len; n++) {
        str[j++]=arr[i++];
    }
    str[len]='\0';
    return str;
}


int main(){
    char sen[1024];
    gets(sen);

    char sen_co[strlen(sen)+1];
    char* pi=(char*)malloc((strlen(sen)+1) + 1);
     if(pi==NULL){ //check if the pointer is null
        exit(1);
    }

    strcpy(sen_co,replace(sen , (strlen(sen)+1), 0,pi));
    free(pi);

    int r=0;
    for (int b= 0; b<strlen(sen)+1; b++){
        if(sen_co[b]==' '){
            r++;
        }
    }
    
    char new[strlen(sen)+1-r];
    int j=0;
    for(int k = 0;k<strlen(sen)+1; k++ ){
        if(sen_co[k]!=' '){
            new[j]=sen_co[k];
            j++;
        }
    }
    
    char str_n[strlen(new)-1];
    char* po=(char*)malloc(strlen(new)-1 + 1);
     if(po==NULL){
        exit(1);
    }
    strcpy(str_n,replace(new , strlen(new)-1, 1,po));
    free(po);

    int i=0;
    for (int j = 0; j < strlen(str_n); j++) {
        if (str_n[j]=='B'||str_n[j]=='D'||str_n[j]=='S'||str_n[j]=='T'||str_n[j]=='\0'|| str_n[j]=='A'){
            break;
        }else{
            i++;
        }
    }

    int c=0;
    char curr[i+1];
    for (int l = 0; l <=i-1 ; l++) {
        curr[c]= str_n[l];
        c++;
    }

    curr[i]='\0';
    Graph *g=build_graph_cmd(curr, i);
    char ans[strlen(str_n)-i];
    char* p=(char*)malloc(strlen(str_n)-i + 1);
     if(p==NULL){
        exit(1);
    }
    strcpy(ans,replace(str_n , (strlen(str_n)-i),i,p)) ;
    free(p);
    ans[strlen(str_n)-i]='\0';
    while (strlen(ans)!=0) {
        if (ans[0] == 'B') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                if (ans[j] == 'B' || ans[j] == 'D' || ans[j] == 'S' || ans[j] == 'T' || ans[j] == '\0' || ans[j] == 'A') {
                    break;
                } else {
                    t++;
                }
            }
           
            char* po=(char*)malloc(t + 1);
             if(po==NULL){
             exit(1);
              }
            create_new_node(replace(ans, t, 1,po), g);
            free(po);
            char* p=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(p==NULL){
            exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,p));
            free(p);
          
        }


        if (ans[0] == 'D') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                if (ans[j] == 'B' || ans[j] == 'D' || ans[j] == 'S' || ans[j] == 'T' || ans[j] == '\0' || ans[j] == 'A') {
                    break;
                } else {
                    t++;
                }
            }

            
            char* po=(char*)malloc(t + 1);
            if(po==NULL){
                exit(1);
            }
            del_node(replace(ans, t, 1,po), g);
            free(po);
            char* p=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(p==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,p));
            free(p);
        }

        if (ans[0] == 'A') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                if (ans[j] == 'B' || ans[j] == 'D' || ans[j] == 'S' || ans[j] == 'T' || ans[j] == '\0'|| ans[j]=='A') {
                    break;
                } else {
                    t++;
                }
            }
            char* po=(char*)malloc(t + 1);
             if(po==NULL){
                exit(1);
            }
            release_graph(g);
            g=build_graph_cmd(replace(ans, t, 1,po), t);
            free(po);
            
            
            char* p=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(p==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,p));
            free(p);
        }


        if (ans[0] == 'S') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                if (ans[j] == 'B' || ans[j] == 'D' || ans[j] == 'S' || ans[j] == 'T' || ans[j] == '\0' || ans[j] == 'A') {
                    break;
                } else {
                    t++;
                }
            }
            char* po=(char*)malloc(t + 1);
            if(po==NULL){
                exit(1);
            }
            Shortest_path(replace(ans, t, 1,po), g);
            free(po);

            char* p=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(p==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,p));
            free(p);
        }


        if (ans[0] == 'T') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                if (ans[j] == 'B' || ans[j] == 'D' || ans[j] == 'S' || ans[j] == 'T' || ans[j] == '\0' || ans[j] == 'A') {
                    break;
                } else {
                    t++;
                }
            }
            char* po=(char*)malloc(t + 1);
             if(po==NULL){
                exit(1);
            }
            TSP_cmd(g,replace(ans, t, 1,po));
            free(po);
            char* p=(char*)malloc((strlen(ans) - t - 1) + 1);
            if(p==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,p));
            free(p);
        }
    }


    release_graph(g);
    

return 0;
}