#include <limits.h>
#include "para.h"


/**********************************************************/
/* parameters of model. Initial values are default values */
/**********************************************************/
int par_maxtime = INT_MAX; /* The maximum time step */
int par_inittime = 0; /* The initial time step */
unsigned long par_ulseed=68765410144; /* In original CASH, seed is int. We need to use
			    unsigned long to use mersenne twister */
double par_kcat = 1.; /* Max Prob. of product formation. Used in
		      parallel.c */
double par_kdec = 0.03; /* Prob. of decay 0.15 for claim 0.03 for normal. For normal, 0.05 seems to be better. */
double par_mutation = 0.008; /* Prob. mutation per base */ /*Local in the new model, see cash2.h*/
double par_init_total = 0.8;
double par_factor_asso = -0.05;
double par_factor_diss = -0.05;
double par_factor_replication = 0.25; /* used in MoleculeActionClaim()*/
double par_factor_rec = 0.02095443156; /*krec=0.6 if energy=-24.38 (the average of random RNA of 100 bases). krec=exp(factor*energy)*/
int par_diff_step = 1; /*# of diffusion steps in one time step */

char par_init_seq[MAXLEN] =   "AAAAAAAAACCGCAACCCAAAGGGAAAGCGAAACGCAGCGGAUUAUAUUU";     /*Initial sequences, used in testworld() for specified populations*/
char par_c_init_seq[MAXLEN] = "AAAUAUAAUCCGCUGCGUUUCGCUUUCCCUUUGGGUUGCGGUUUUUUUUU";
char par_target_structure[MAXLEN] = "..............(((.(....)..(....).)))..............";

//char Ccat[MAXLEN] = "CCCCCCCCCCCCCCGGCCGGAAACAACGUAAGAGCCAUUGUGUGGAUGCC";       /*Nobuto's C cat*/
//char c_Ccat[MAXLEN]="GGCAUCCACACAAUGGCUCUUACGUUGUUUCCGGCCGGGGGGGGGGGGGG";

char Ccat[MAXLEN] = "CCCCCCCCCCCCCGACACGGAAACGACGUGAGAGUCAUUAGAUAGGUGUC";	/*Sandro cat 1*/
char c_Ccat[MAXLEN]="GACACCUAUCUAAUGACUCUCACGUCGUUUCCGUGUCGGGGGGGGGGGGG";

int par_n_graph = 10; /* the # of the complimentary pairs in graph. #
		   of node will be twice of this */
int par_graph_interval = 1000; /* make it INT_MAX to turn it off
				  */
int par_out_sequence_interval = 2500;	/*period for outputting sequence-prototype.seq*/
int timestep = 10000;			/*period for outputting pmutatis.txt and mmutatis.txt*/
double par_k_threshold = 0.5; /* */
/* if match>mthreshold, then draw arrow. It is also used in
    parallel.c */
int par_m_threshold = 15;
/* if NOT match > par_m_threshold_parasite, then the molecule
   will not be replicated given that PARASITE_NOT_REPLICATED is
   turned on. The reason we separate par_m_threshold and
   par_m_threshold_parasite is that we */
int par_m_threshold_parasite = 4;
int par_if_display = 0; /*If it is more than 0, the program displays CA.*/
int par_display_interval = 1; /* interval of display */
int par_movie_interval = 10; /* the interval of movie */
int par_if_movie = 1; /* if 1, make movie */
int par_backup_interval = 50000; /* interval of back up */
int par_xmgrace_interval = 1;
int par_cash_size = 800; /* Mb. Approximately */

/* The maximimum distance to allow replication in null model. */
int par_max_dist_replicate_in_null = 3; 

int par_n_genotype_plot = 10; /* the # of genotypes of which pop size is
				 displayed. Not more than 15!! */
int par_n_phenotype_color_cat = 20; /* Top n catalyst to put the color. */
int par_n_phenotype_color_par = 20; /* Top n parasite to put the color. */
/* [0,par_color_size] will be the index range of the color. Color
   0 is background and should be black. Color 1 is the color of
   undefined phenotype, and preferably white. In Xmgrace
   plotting, color [1,par_color_size] will be used. Color 1 in
   Xmgrace will be black instead of white.
*/
unsigned int par_color_size = 215; 

/* The number of parasites and catalysts to output the
   sequence. */
int par_n_para_output_seq = 200;
int par_n_cata_output_seq = 200;

int par_margin = 5; /* margin of display */
/* The color of undetermined function. The color of uncolored
   phenotype. The color of undetermined phenotype. */
unsigned int par_color_undefined = 1; 
unsigned int par_color_symmetric = 179;    /* (0, 255, 0) */
unsigned int par_color_asymmetric_c = 148; /* (255, 0, 0) */
unsigned int par_color_asymmetric_t = 95;  /* (0, 0, 255) */
unsigned int par_color_parasite = 135;     /* (255,204,0) */


/****************************************/
/* basic parameters irrelevant of model */
/****************************************/
char *par_version = "Prototype 3: Wave";
char par_grace_outfile_name[BUFSIZE]="plot-prototype";
char endFileNameG[BUFSIZE]="";
char par_graph_outfile_name[BUFSIZE]="graph-prototype";
char par_graph_outfile_name2[BUFSIZE]="graph-prototype2";
char par_backup_file_name[BUFSIZE]="backup-prototype";
char par_backup_directory_name[BUFSIZE]="backup";
char par_movie_directory_name[BUFSIZE]="movie-prototype";
char par_error_backup_file_name[BUFSIZE]="error-prototype";
char par_input_file_name[BUFSIZE]="";
char par_sequence_file_name[BUFSIZE]="sequence-prototype.seq";
