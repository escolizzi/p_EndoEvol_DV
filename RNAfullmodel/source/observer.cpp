#include <vector>
#include <string>

#include "p-observer.hpp"
#include "g-observer.hpp"
#include "plot_grace.hpp"
#include "display.hpp"

extern "C"{
#include "cash2.h"
#include "para.h"
}

#include "observer.hpp"

extern int nrow;
extern int ncol;

void Observer::observe(const TYPE2 **world)
{

  /* Set all genotype population to zero */
  g_observer.setAllPopZero();

  /* Insert all replicators in g_observer. */
  int nr = nrow;
  int nc = ncol;
  for(int i=1;i<=nr;i++)
    for(int j=1;j<=nc;j++){
      if(world[i][j].seq[0]!='N'){
	g_observer.insert(world[i][j].seq,world[i][j].str,world[i][j].end3,world[i][j].end5,world[i][j].cseq,world[i][j].cstr,world[i][j].cend3,world[i][j].cend5,world[i][j].kcat);
      }
    }

  /* Delete all genotypes of which population is zero. */
  g_observer.deleteZeroPopGenotype();

  /* Set all phenotype population to zero. */
  p_observer.setAllPopZero();

  /* Input all data of g_observer to p_observer. */
  g_observer.communicatePobserver(p_observer);

  /* P_observer makes a ranking of phenotype */
  p_observer.makeRanking();
}

void Observer::plotPopulation(int time)
{
  std::vector<int> func_pop,p_pop,m_pop;
  std::vector<unsigned int> color_vec;
  std::vector<bool> if_catalyst_vec;
  
  p_observer.getPopulation(func_pop,color_vec,p_pop,m_pop,if_catalyst_vec);
  plot_grace.plotPhenotype(time,color_vec,p_pop,m_pop,if_catalyst_vec);
  plot_grace.plotFunction(time,func_pop[0],func_pop[1],func_pop[2],func_pop[3],func_pop[4]);
}

void Observer::outputSequences(int time)
{
  p_observer.outputSequences(time);
}

void Observer::updateDisplayMovieColorByComposition(const TYPE2 **world, struct point ***neighbor)
{
  int nr = nrow;
  int nc = ncol;
  unsigned int color_func;
  unsigned int color_phen;
  for(int i=1;i<=nr;i++)
    for(int j=1;j<=nc;j++){
      p_observer.getColorByComposition(i,j,color_func,color_phen,world[i][j].seq,world[i][j].str,world[i][j].end3,world[i][j].end5,world[i][j].cstr,neighbor,world);
      //std::cout<<"Color func: "<<color_func<<". Color phen: "<<color_phen<<"\n";
      
      display.set_func(i,j,color_func);
      display.set_phen(i,j,color_phen);
    }

  if(::par_if_display == 1)
    display.draw_display();
  if(::par_if_movie == 1)
    display.draw_movie();
}

void Observer::main(int time,const TYPE2 **world, struct point ***neighbor)
{
  if(time % ::par_xmgrace_interval==0){
    /* Get the data from the plane */
    observe(world);

    /* Output the population to xmgrace */
    //plotPopulation(time);

    /* Save xmgrace file once in a while */
    //if(time % (::par_xmgrace_interval * 10)==0)
    //  plot_grace.save();

    if(time % ::par_out_sequence_interval == 0)
      outputSequences(time);
    
    if((::par_if_display == 1 && time % ::par_display_interval==0)||(::par_if_movie == 1 && time % ::par_movie_interval==0))
      updateDisplayMovieColorByComposition(world, neighbor);
  }
}
