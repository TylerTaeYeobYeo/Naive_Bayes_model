#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/libstemmer.h"

#define NEGATIVE 9078
#define NON_NEGATIVE 5565

FILE * model;
GHashTable * stop; //stopwords

void print_csv (gpointer key, gpointer value, gpointer userdata) 
{
	char * t = key ;
	int * d = value ;
    fprintf(model,"%s %0.32lf %0.32lf\n", t, (double)d[0]/NEGATIVE, (double)d[1]/NON_NEGATIVE);
	// printf("%s %lf %lf\n", t, (double)d[0]/Negative, (double)d[1]/nonNegative) ;
}

int check(const char *s){
    //hashtags
    int * d ;			
    d = g_hash_table_lookup(stop, s);
    if(d==NULL) return 1;
    return 0;
}

void SWDictionary(GHashTable * dic){
    FILE *fp = fopen("../lib/stopwords","r");
    char buf[64] = "";
    while(!feof(fp)){
        fscanf(fp,"%s",buf);
        if(buf[0]=='#') continue;
        int *d = malloc(sizeof(short));
        *d = 1;
        g_hash_table_insert(dic,strdup(buf),d);
    }
    fclose(fp);
}

void read(FILE *f, GHashTable * counter, int index){
    char * line = 0x0 ;
	// size_t r ; 
	size_t n = 0;

    struct sb_stemmer * stemmer ;

	stemmer = sb_stemmer_new("english", 0x0) ;

    while (getline(&line, &n, f) >= 0) {
		char * t ;
		char * _line = line ;
		for (t = strtok(line, " \n\t"); t != 0x0 ; t = strtok(0x0, " \n\t")) { 
            int src = 0, dest = 0;
            //lowercase && remove special chars
            while (t[src] != '\0') {
                if (isalpha(t[src])) {
                    t[dest] = tolower(t[src]);
                    ++dest;
                }
                ++src;
            }
            t[dest] = '\0';
            const char *s = sb_stemmer_stem(stemmer, t, strlen(t));
            // printf("%s %s\n",t,s);
            //remove stopwords
            if(strlen(s)>0 && check(s)){
                int * d ;			
                d = g_hash_table_lookup(counter, s) ;
                if (d == NULL) {
                    d = malloc(sizeof(int)*2) ;
                    if(index){
                        d[0] = 0;
                        d[1] = 1;
                    }
                    else{
                        d[0] = 1;
                        d[1] = 0;
                    }
                    g_hash_table_insert(counter, strdup(s), d);
                }
                else {
                    d[index]++;
                }
            }
		}
		free(_line) ;
		line = 0x0 ;
	}
	sb_stemmer_delete(stemmer) ;
}

int main () 
{
	GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;
    stop = g_hash_table_new(g_str_hash, g_str_equal);

    SWDictionary(stop);

	char * line = 0x0 ;
	// size_t r ; 
	size_t n = 0 ;
	
    //negative case
    FILE * f = fopen("../data/train.negative.csv", "r") ;
	read(f,counter,0);
	fclose(f) ;

    //non negative case
    f = fopen("../data/train.non-negative.csv", "r");
    read(f,counter,1);
    fclose(f);

    //print out to model.csv
    model = fopen("./model.csv","w");
	g_hash_table_foreach(counter, print_csv, 0x0) ;
    fclose(model);

    // printf("%lf\n",(double)1/NEGATIVE);
    return 0;
}
