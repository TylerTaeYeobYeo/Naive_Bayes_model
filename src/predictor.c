#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/libstemmer.h"
#include <math.h>


/* to change thresh hold, change this value*/
#define COMP 0.5

void readModel(FILE *f, GHashTable * counter){
    char * line = 0x0 ;
	// size_t r ; 
	size_t n = 0;
    char buf[64];
     while(fscanf(f,"%s",buf)!=EOF){
        double* d = malloc(sizeof(double)*2);
        fscanf(f,"%lf",&d[0]);
        fscanf(f,"%lf",&d[1]);
        g_hash_table_insert(counter, strdup(buf), d);
        // printf("%s %lf %lf\n",buf,d[0],d[1]);
    };
}

/* This function test out the data file with the dictionary and gives result of how many were negatives
 * *f for test data file
 * counter for hash table (aka dictionary)
 * thresh_hold compare value
 */
int testData(FILE *f, GHashTable * counter, double thresh_hold){
	char * line = 0x0 ;
	size_t n = 0 ;
    struct sb_stemmer * stemmer ;
    stemmer = sb_stemmer_new("english", 0x0) ;

    int result = 0;

	while (getline(&line, &n, f) >= 0) {
        char * line = 0x0 ;
        size_t n = 0;

        while (getline(&line, &n, f) >= 0) {
            char * t ;
            double s_m, m_s = 1, m_ns = 1;
            double l_m_s = 0, l_m_ns = 0; //log

            for (t = strtok(line, " \n\t"); t != 0x0 ; t = strtok(0x0, " \n\t")) { 
                /*pre-processing on word*/
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

                /* look-up */
                double * d = g_hash_table_lookup(counter, s) ;
                if (d != NULL) {
                    // m_s *= d[0];
                    // m_ns *= d[1];
                    l_m_s += log(d[0]);
                    l_m_ns += log(d[1]);
                }
            }
            m_s = exp(l_m_s);
            m_ns = exp(l_m_ns);

            s_m = m_s / (m_s + m_ns);

            if(s_m > thresh_hold) result++;
            line = 0x0;
        }
	}
    sb_stemmer_delete(stemmer);
    return result;
}

int main(){
    GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal);
    
    //read model csv and make into hash table.
    FILE *fp = fopen("./model.csv","r");
    readModel(fp,counter);
    fclose(fp);

    /* test negative datas*/
    fp = fopen("../data/test.negative.csv","r");
    int neg = testData(fp, counter, COMP);
    printf("test negative:      %d/100\n", neg);
    fclose(fp);

    /* test non-negative datas*/
    fp = fopen("../data/test.non-negative.csv","r");
    int non_neg = 100 - testData(fp, counter, COMP);
    printf("test non-negative:  %d/100\n", non_neg);
    fclose(fp);

    return 0;
}