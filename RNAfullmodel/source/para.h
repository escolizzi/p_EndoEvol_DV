#include "constants.h"

#ifndef PARAMETER
#define PARAMETER
/**********************************************************/
/* parameters of model. Initial values are default values */
/**********************************************************/
extern int par_maxtime;
extern int par_inittime;
extern unsigned long par_ulseed;
extern double par_kcat;
extern double par_kdec;
extern double par_mutation;
extern double par_init_total;
extern double par_factor_asso;
extern double par_factor_diss;
extern double par_factor_replication;
extern double par_factor_rec;
extern int par_diff_step;

extern char par_init_seq[MAXLEN];
extern char par_c_init_seq[MAXLEN];
extern char par_target_structure[MAXLEN];
extern char Ccat[MAXLEN];
extern char c_Ccat[MAXLEN];

//extern int par_n_graph;
//extern int par_graph_interval;
extern int timestep;
extern int par_out_sequence_interval;

extern double par_k_threshold;
extern int par_m_threshold;
extern int par_m_threshold_parasite;

extern int par_if_display;
extern int par_display_interval;
extern int par_movie_interval;
extern int par_if_movie;
extern int par_backup_interval;
extern int par_xmgrace_interval;
extern int par_cash_size;

extern int par_max_dist_replicate_in_null;

//extern int par_n_genotype_plot;

extern unsigned int par_color_size;

extern int par_n_para_output_seq;
extern int par_n_cata_output_seq;

extern int par_n_phenotype_color_cat;
extern int par_n_phenotype_color_par;

extern int par_margin;
extern unsigned int par_color_undefined;
extern unsigned int par_color_symmetric;
extern unsigned int par_color_asymmetric_c;
extern unsigned int par_color_asymmetric_t;
extern unsigned int par_color_parasite;


/****************************************/
/* basic parameters irrelevant of model */
/****************************************/
extern char *par_version;
extern char par_grace_outfile_name[BUFSIZE];
extern char endFileNameG[BUFSIZE];
extern char par_graph_outfile_name[BUFSIZE];
extern char par_graph_outfile_name2[BUFSIZE];
extern char par_backup_file_name[BUFSIZE];
extern char par_backup_directory_name[BUFSIZE];
extern char par_movie_directory_name[BUFSIZE];
extern char par_error_backup_file_name[BUFSIZE];
extern char par_input_file_name[BUFSIZE];
extern char par_sequence_file_name[BUFSIZE];

#endif //PARAMETER
