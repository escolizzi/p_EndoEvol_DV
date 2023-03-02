#ifndef RNASTRING
#define RNASTRING

/* It returns the replicated rna sequence. */
int Replication(char* child,char* parent,double mutationrate);
void PerfectReplication(char* child,char* parent);
int IfcRNA(char* sequence1,char* sequence2);
/* RNAi2c, c2i and c2cc */
char RNAi2c(int rna_int); /* RNA integer to RNA character */
int RNAc2i(char rna_char); /* RNA character to RNA integer */
char RNAc2cc(char rna_char); /* RNA chara to complementary RNA chara */

#endif // RNASTRING
