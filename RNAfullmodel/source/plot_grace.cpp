#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
extern "C"{
#include <grace_np.h>
}

extern "C"{
#include "para.h"
}
#include "plot_grace.hpp"

PlotGrace::PlotGrace()
{
  return;

  if(par_if_display==1){
    if(::GraceOpenVA("xmgrace",16384,"-noask","-nosafe",NULL) == -1){
      std::cerr << "PlotGrace::PlotGrace() I cannot run Xmgrace." << std::endl;
      std::exit(-1);
    }
  }else{
    if(::GraceOpenVA("gracebat",16384,"-noask","-nosafe",NULL) == -1){
      std::cerr << "PlotGrace::PlotGrace() I cannot run ::Gracebat." << std::endl;
      std::exit(-1);
    }
  }

  /* g0 is a graph for population. */

  ::GracePrintf("map color 0 to (255, 255, 255), \"0\"");
  ::GracePrintf("map color 2 to (51, 51, 152), \"2\"");
  ::GracePrintf("map color 3 to (51, 102, 102), \"3\"");
  ::GracePrintf("map color 4 to (0, 51, 255), \"4\"");
  ::GracePrintf("map color 5 to (51, 204, 255), \"5\"");
  ::GracePrintf("map color 6 to (255, 152, 102), \"6\"");
  ::GracePrintf("map color 7 to (84, 84, 84), \"7\"");
  ::GracePrintf("map color 8 to (204, 102, 204), \"8\"");
  ::GracePrintf("map color 9 to (0, 152, 152), \"9\"");
  ::GracePrintf("map color 10 to (204, 204, 152), \"10\"");
  ::GracePrintf("map color 11 to (204, 102, 255), \"11\"");
  ::GracePrintf("map color 12 to (102, 0, 152), \"12\"");
  ::GracePrintf("map color 13 to (51, 0, 0), \"13\"");
  ::GracePrintf("map color 14 to (255, 152, 255), \"14\"");
  ::GracePrintf("map color 15 to (135, 135, 135), \"15\"");
  ::GracePrintf("map color 16 to (51, 102, 152), \"16\"");
  ::GracePrintf("map color 17 to (0, 204, 0), \"17\"");
  ::GracePrintf("map color 18 to (0, 255, 204), \"18\"");
  ::GracePrintf("map color 19 to (152, 204, 255), \"19\"");
  ::GracePrintf("map color 20 to (255, 102, 51), \"20\"");
  ::GracePrintf("map color 21 to (152, 51, 0), \"21\"");
  ::GracePrintf("map color 22 to (51, 51, 255), \"22\"");
  ::GracePrintf("map color 23 to (152, 51, 102), \"23\"");
  ::GracePrintf("map color 24 to (0, 255, 255), \"24\"");
  ::GracePrintf("map color 25 to (204, 152, 152), \"25\"");
  ::GracePrintf("map color 26 to (0, 204, 255), \"26\"");
  ::GracePrintf("map color 27 to (0, 255, 0), \"27\"");
  ::GracePrintf("map color 28 to (255, 51, 0), \"28\"");
  ::GracePrintf("map color 29 to (255, 204, 204), \"29\"");
  ::GracePrintf("map color 30 to (0, 51, 152), \"30\"");
  ::GracePrintf("map color 31 to (51, 152, 255), \"31\"");
  ::GracePrintf("map color 32 to (0, 102, 102), \"32\"");
  ::GracePrintf("map color 33 to (102, 204, 204), \"33\"");
  ::GracePrintf("map color 34 to (152, 152, 51), \"34\"");
  ::GracePrintf("map color 35 to (102, 255, 0), \"35\"");
  ::GracePrintf("map color 36 to (51, 51, 204), \"36\"");
  ::GracePrintf("map color 37 to (102, 51, 0), \"37\"");
  ::GracePrintf("map color 38 to (0, 0, 255), \"38\"");
  ::GracePrintf("map color 39 to (255, 51, 152), \"39\"");
  ::GracePrintf("map color 40 to (102, 152, 204), \"40\"");
  ::GracePrintf("map color 41 to (51, 204, 0), \"41\"");
  ::GracePrintf("map color 42 to (152, 0, 255), \"42\"");
  ::GracePrintf("map color 43 to (0, 204, 51), \"43\"");
  ::GracePrintf("map color 44 to (152, 51, 152), \"44\"");
  ::GracePrintf("map color 45 to (204, 102, 0), \"45\"");
  ::GracePrintf("map color 46 to (102, 102, 102), \"46\"");
  ::GracePrintf("map color 47 to (204, 0, 255), \"47\"");
  ::GracePrintf("map color 48 to (102, 204, 255), \"48\"");
  ::GracePrintf("map color 49 to (255, 152, 152), \"49\"");
  ::GracePrintf("map color 50 to (0, 102, 255), \"50\"");
  ::GracePrintf("map color 51 to (255, 51, 102), \"51\"");
  ::GracePrintf("map color 52 to (102, 152, 102), \"52\"");
  ::GracePrintf("map color 53 to (204, 0, 102), \"53\"");
  ::GracePrintf("map color 54 to (51, 204, 51), \"54\"");
  ::GracePrintf("map color 55 to (204, 0, 51), \"55\"");
  ::GracePrintf("map color 56 to (152, 102, 204), \"56\"");
  ::GracePrintf("map color 57 to (152, 102, 102), \"57\"");
  ::GracePrintf("map color 58 to (204, 51, 152), \"58\"");
  ::GracePrintf("map color 59 to (204, 152, 255), \"59\"");
  ::GracePrintf("map color 60 to (204, 51, 102), \"60\"");
  ::GracePrintf("map color 61 to (102, 255, 255), \"61\"");
  ::GracePrintf("map color 62 to (33, 33, 33), \"62\"");
  ::GracePrintf("map color 63 to (51, 102, 51), \"63\"");
  ::GracePrintf("map color 64 to (102, 0, 102), \"64\"");
  ::GracePrintf("map color 65 to (0, 102, 204), \"65\"");
  ::GracePrintf("map color 66 to (51, 0, 204), \"66\"");
  ::GracePrintf("map color 67 to (152, 152, 152), \"67\"");
  ::GracePrintf("map color 68 to (204, 204, 102), \"68\"");
  ::GracePrintf("map color 69 to (255, 0, 255), \"69\"");
  ::GracePrintf("map color 70 to (0, 152, 51), \"70\"");
  ::GracePrintf("map color 71 to (152, 51, 204), \"71\"");
  ::GracePrintf("map color 72 to (204, 255, 204), \"72\"");
  ::GracePrintf("map color 73 to (119, 119, 119), \"73\"");
  ::GracePrintf("map color 74 to (255, 0, 51), \"74\"");
  ::GracePrintf("map color 75 to (51, 204, 204), \"75\"");
  ::GracePrintf("map color 76 to (0, 51, 0), \"76\"");
  ::GracePrintf("map color 77 to (255, 204, 102), \"77\"");
  ::GracePrintf("map color 78 to (152, 204, 204), \"78\"");
  ::GracePrintf("map color 79 to (51, 204, 152), \"79\"");
  ::GracePrintf("map color 80 to (102, 51, 152), \"80\"");
  ::GracePrintf("map color 81 to (170, 170, 170), \"81\"");
  ::GracePrintf("map color 82 to (102, 255, 102), \"82\"");
  ::GracePrintf("map color 83 to (255, 102, 102), \"83\"");
  ::GracePrintf("map color 84 to (51, 152, 0), \"84\"");
  ::GracePrintf("map color 85 to (51, 255, 204), \"85\"");
  ::GracePrintf("map color 86 to (0, 102, 51), \"86\"");
  ::GracePrintf("map color 87 to (152, 102, 255), \"87\"");
  ::GracePrintf("map color 88 to (255, 102, 255), \"88\"");
  ::GracePrintf("map color 89 to (0, 51, 102), \"89\"");
  ::GracePrintf("map color 90 to (255, 102, 152), \"90\"");
  ::GracePrintf("map color 91 to (186, 186, 186), \"91\"");
  ::GracePrintf("map color 92 to (51, 204, 102), \"92\"");
  ::GracePrintf("map color 93 to (255, 0, 204), \"93\"");
  ::GracePrintf("map color 94 to (0, 51, 204), \"94\"");
  ::GracePrintf("map color 95 to (204, 255, 51), \"95\"");
  ::GracePrintf("map color 96 to (0, 0, 204), \"96\"");
  ::GracePrintf("map color 97 to (102, 204, 102), \"97\"");
  ::GracePrintf("map color 98 to (204, 255, 0), \"98\"");
  ::GracePrintf("map color 99 to (255, 102, 204), \"99\"");
  ::GracePrintf("map color 100 to (152, 102, 51), \"100\"");
  ::GracePrintf("map color 101 to (152, 0, 102), \"101\"");
  ::GracePrintf("map color 102 to (255, 152, 0), \"102\"");
  ::GracePrintf("map color 103 to (204, 255, 152), \"103\"");
  ::GracePrintf("map color 104 to (102, 255, 51), \"104\"");
  ::GracePrintf("map color 105 to (102, 0, 204), \"105\"");
  ::GracePrintf("map color 106 to (102, 51, 255), \"106\"");
  ::GracePrintf("map color 107 to (0, 204, 204), \"107\"");
  ::GracePrintf("map color 108 to (0, 102, 0), \"108\"");
  ::GracePrintf("map color 109 to (204, 255, 255), \"109\"");
  ::GracePrintf("map color 110 to (152, 152, 102), \"110\"");
  ::GracePrintf("map color 111 to (238, 8, 19), \"111\"");
  ::GracePrintf("map color 112 to (152, 102, 152), \"112\"");
  ::GracePrintf("map color 113 to (152, 102, 0), \"113\"");
  ::GracePrintf("map color 114 to (0, 204, 102), \"114\"");
  ::GracePrintf("map color 115 to (51, 102, 204), \"115\"");
  ::GracePrintf("map color 116 to (255, 255, 102), \"116\"");
  ::GracePrintf("map color 117 to (0, 152, 102), \"117\"");
  ::GracePrintf("map color 118 to (152, 152, 0), \"118\"");
  ::GracePrintf("map color 119 to (255, 152, 51), \"119\"");
  ::GracePrintf("map color 120 to (204, 51, 255), \"120\"");
  ::GracePrintf("map color 121 to (255, 0, 152), \"121\"");
  ::GracePrintf("map color 122 to (51, 152, 102), \"122\"");
  ::GracePrintf("map color 123 to (0, 152, 204), \"123\"");
  ::GracePrintf("map color 124 to (102, 204, 152), \"124\"");
  ::GracePrintf("map color 125 to (62, 56, 10), \"125\"");
  ::GracePrintf("map color 126 to (102, 51, 102), \"126\"");
  ::GracePrintf("map color 127 to (0, 51, 51), \"127\"");
  ::GracePrintf("map color 128 to (51, 0, 102), \"128\"");
  ::GracePrintf("map color 129 to (102, 255, 204), \"129\"");
  ::GracePrintf("map color 130 to (204, 204, 0), \"130\"");
  ::GracePrintf("map color 131 to (255, 0, 0), \"131\"");
  ::GracePrintf("map color 132 to (204, 0, 152), \"132\"");
  ::GracePrintf("map color 133 to (0, 152, 0), \"133\"");
  ::GracePrintf("map color 134 to (255, 152, 204), \"134\"");
  ::GracePrintf("map color 135 to (255, 255, 51), \"135\"");
  ::GracePrintf("map color 136 to (204, 204, 255), \"136\"");
  ::GracePrintf("map color 137 to (255, 255, 152), \"137\"");
  ::GracePrintf("map color 138 to (51, 152, 51), \"138\"");
  ::GracePrintf("map color 139 to (152, 152, 255), \"139\"");
  ::GracePrintf("map color 140 to (221, 221, 221), \"140\"");
  ::GracePrintf("map color 141 to (204, 102, 152), \"141\"");
  ::GracePrintf("map color 142 to (152, 204, 102), \"142\"");
  ::GracePrintf("map color 143 to (51, 51, 102), \"143\"");
  ::GracePrintf("map color 144 to (204, 102, 51), \"144\"");
  ::GracePrintf("map color 145 to (0, 255, 102), \"145\"");
  ::GracePrintf("map color 146 to (152, 0, 51), \"146\"");
  ::GracePrintf("map color 147 to (255, 51, 255), \"147\"");
  ::GracePrintf("map color 148 to (255, 51, 204), \"148\"");
  ::GracePrintf("map color 149 to (204, 152, 204), \"149\"");
  ::GracePrintf("map color 150 to (51, 255, 51), \"150\"");
  ::GracePrintf("map color 151 to (152, 0, 152), \"151\"");
  ::GracePrintf("map color 152 to (152, 255, 0), \"152\"");
  ::GracePrintf("map color 153 to (102, 204, 0), \"153\"");
  ::GracePrintf("map color 154 to (255, 204, 51), \"154\"");
  ::GracePrintf("map color 155 to (204, 0, 204), \"155\"");
  ::GracePrintf("map color 156 to (0, 0, 51), \"156\"");
  ::GracePrintf("map color 157 to (152, 204, 0), \"157\"");
  ::GracePrintf("map color 158 to (255, 255, 204), \"158\"");
  ::GracePrintf("map color 159 to (102, 204, 51), \"159\"");
  ::GracePrintf("map color 160 to (204, 102, 102), \"160\"");
  ::GracePrintf("map color 161 to (204, 51, 51), \"161\"");
  ::GracePrintf("map color 162 to (102, 0, 255), \"162\"");
  ::GracePrintf("map color 163 to (204, 0, 0), \"163\"");
  ::GracePrintf("map color 164 to (102, 102, 204), \"164\"");
  ::GracePrintf("map color 165 to (102, 51, 204), \"165\"");
  ::GracePrintf("map color 166 to (102, 152, 51), \"166\"");
  ::GracePrintf("map color 167 to (0, 102, 152), \"167\"");
  ::GracePrintf("map color 168 to (255, 204, 152), \"168\"");
  ::GracePrintf("map color 169 to (102, 152, 152), \"169\"");
  ::GracePrintf("map color 170 to (51, 152, 204), \"170\"");
  ::GracePrintf("map color 171 to (255, 0, 255), \"171\"");
  ::GracePrintf("map color 172 to (51, 102, 0), \"172\"");
  ::GracePrintf("map color 173 to (102, 16, 1), \"173\"");
  ::GracePrintf("map color 174 to (255, 51, 51), \"174\"");
  ::GracePrintf("map color 175 to (152, 0, 204), \"175\"");
  ::GracePrintf("map color 176 to (204, 152, 102), \"176\"");
  ::GracePrintf("map color 177 to (152, 51, 51), \"177\"");
  ::GracePrintf("map color 178 to (152, 204, 51), \"178\"");
  ::GracePrintf("map color 179 to (51, 0, 51), \"179\"");
  ::GracePrintf("map color 180 to (102, 102, 255), \"180\"");
  ::GracePrintf("map color 181 to (204, 255, 102), \"181\"");
  ::GracePrintf("map color 182 to (102, 102, 51), \"182\"");
  ::GracePrintf("map color 183 to (51, 0, 152), \"183\"");
  ::GracePrintf("map color 184 to (255, 102, 0), \"184\"");
  ::GracePrintf("map color 185 to (0, 204, 152), \"185\"");
  ::GracePrintf("map color 186 to (152, 204, 152), \"186\"");
  ::GracePrintf("map color 187 to (0, 255, 152), \"187\"");
  ::GracePrintf("map color 188 to (152, 255, 51), \"188\"");
  ::GracePrintf("map color 189 to (204, 204, 51), \"189\"");
  ::GracePrintf("map color 190 to (152, 255, 152), \"190\"");
  ::GracePrintf("map color 191 to (237, 237, 237), \"191\"");
  ::GracePrintf("map color 192 to (255, 0, 102), \"192\"");
  ::GracePrintf("map color 193 to (102, 152, 0), \"193\"");
  ::GracePrintf("map color 194 to (204, 152, 51), \"194\"");
  ::GracePrintf("map color 195 to (152, 0, 0), \"195\"");
  ::GracePrintf("map color 196 to (68, 68, 68), \"196\"");
  ::GracePrintf("map color 197 to (51, 255, 255), \"197\"");
  ::GracePrintf("map color 198 to (255, 255, 0), \"198\"");
  ::GracePrintf("map color 199 to (51, 102, 255), \"199\"");
  ::GracePrintf("map color 200 to (102, 51, 51), \"200\"");
  ::GracePrintf("map color 201 to (255, 204, 0), \"201\"");
  ::GracePrintf("map color 202 to (152, 51, 255), \"202\"");
  ::GracePrintf("map color 203 to (102, 152, 255), \"203\"");
  ::GracePrintf("map color 204 to (0, 255, 51), \"204\"");
  ::GracePrintf("map color 205 to (152, 255, 102), \"205\"");
  ::GracePrintf("map color 206 to (51, 51, 51), \"206\"");
  ::GracePrintf("map color 207 to (102, 255, 152), \"207\"");
  ::GracePrintf("map color 208 to (204, 51, 204), \"208\"");
  ::GracePrintf("map color 209 to (204, 51, 0), \"209\"");
  ::GracePrintf("map color 210 to (152, 255, 255), \"210\"");
  ::GracePrintf("map color 211 to (0, 0, 102), \"211\"");
  ::GracePrintf("map color 212 to (51, 255, 152), \"212\"");
  ::GracePrintf("map color 213 to (51, 152, 152), \"213\"");
  ::GracePrintf("map color 214 to (152, 255, 204), \"214\"");
  ::GracePrintf("map color 215 to (102, 102, 152), \"215\"");
  ::GracePrintf("map color 216 to (102, 102, 0), \"216\"");
  ::GracePrintf("map color 217 to (102, 0, 51), \"217\"");
  ::GracePrintf("map color 218 to (0, 155, 255), \"218\"");
  ::GracePrintf("map color 219 to (152, 152, 204), \"219\"");
  ::GracePrintf("map color 220 to (51, 255, 102), \"220\"");
  ::GracePrintf("map color 221 to (0, 0, 152), \"221\"");
  ::GracePrintf("map color 222 to (51, 255, 0), \"222\"");
  ::GracePrintf("map color 223 to (204, 152, 0), \"223\"");
  ::GracePrintf("map color 224 to (51, 0, 255), \"224\"");
  ::GracePrintf("map color 225 to (204, 204, 204), \"225\"");

  ::GracePrintf("g0 on");
  ::GracePrintf("with g0");

  ::GracePrintf("view xmin 0.15");
  ::GracePrintf("view xmax 1.265126");
  ::GracePrintf("view ymin 0.55");
  ::GracePrintf("view ymax 0.9");

  ::GracePrintf("focus g0");

  ::GracePrintf("title \"Prototype\"");

  ::GracePrintf("legend on");
  ::GracePrintf("legend box linestyle 0");
  ::GracePrintf("legend 0.17, 1.01");

  ::GracePrintf("s0 on");
  ::GracePrintf("s0 color 1");
  ::GracePrintf("s0 legend \"Total\"");

  ::GracePrintf("s1 on");
  ::GracePrintf("s1 color %d",::par_color_symmetric);
  ::GracePrintf("s1 legend \"Sym-catalysts\"");

  ::GracePrintf("s2 on");
  ::GracePrintf("s2 color %d",::par_color_asymmetric_c);
  ::GracePrintf("s2 legend \"Asym-cat functional\"");

  ::GracePrintf("s3 on");
  ::GracePrintf("s3 color %d",::par_color_asymmetric_t);
  ::GracePrintf("s3 legend \"Asym-cat not-funct.\"");

  ::GracePrintf("s4 on");
  ::GracePrintf("s4 color %d",::par_color_parasite);
  ::GracePrintf("s4 legend \"Parasite\"");

  ::GracePrintf("s5 on");
  ::GracePrintf("s5 color 1");
  ::GracePrintf("s5 legend \"Undetermined\"");

  /* g1 is a graph for catalysts population. */

  ::GracePrintf("g1 on");
  ::GracePrintf("with g1");

  ::GracePrintf("view xmin 0.15");
  ::GracePrintf("view xmax 1.265126");
  ::GracePrintf("view ymin 0.1");
  ::GracePrintf("view ymax 0.45");

  ::GracePrintf("focus g1");

  /* The index of sets and the color corresponds to each
     other. There are in total 'par_color_size*2-1' sets of
     data. Set[0] does not correspond anything. Set[1] is for
     plus string of the undefined
     phenotype. Set[1+par_color_size] is for minus string of
     the undefined phenotype. Set[2] is for the plus phenotype
     of color 2. Set[2+par_color_size] is for the minus
     phenotype of color 2.

     Set 2 has color 2. If n>par_color_size, then Set n has
     color n-par_color_size. If n=par_color_size, then
     color is 1. */
  for(unsigned int i=1;i<par_color_size+1;i++){
    ::GracePrintf("s%d on",i);
    ::GracePrintf("s%d color %d",i,i);
    ::GracePrintf("s%d on",i+par_color_size);
    ::GracePrintf("s%d color %d",i+par_color_size,i);
  }


 /* g2 is a graph for parasites population. */

  ::GracePrintf("g2 on");
  ::GracePrintf("with g2");

  ::GracePrintf("view xmin 0.15");
  ::GracePrintf("view xmax 1.265126");
  ::GracePrintf("view ymin 0.1");
  ::GracePrintf("view ymax 0.45");

  ::GracePrintf("focus g2");
  for(unsigned int i=1;i<par_color_size+1;i++){
    ::GracePrintf("s%d on",i);
    ::GracePrintf("s%d color %d",i,i);
    ::GracePrintf("s%d on",i+par_color_size);
    ::GracePrintf("s%d color %d",i+par_color_size,i);
  }
}



PlotGrace::~PlotGrace()
{
  if(::GraceIsOpen()){
    ::GraceFlush();
    /* Tell Grace to save the data */
    ::GracePrintf("saveall \"%s.agr\"",par_grace_outfile_name);
    
    /* Flush the output buffer and close Grace */
    ::GraceClose();
  }
  else{
    std::cerr << "TermXmgrace: grace is not running." << std::endl;
  }
}


void PlotGrace::plotPhenotype(int time,const std::vector<unsigned> &color_vec,const std::vector<int> &p_pop_vec,const std::vector<int> &m_pop_vec,const std::vector<bool> &if_catalyst_vec)
{
  if(p_pop_vec.size()!=m_pop_vec.size() || if_catalyst_vec.size()!=color_vec.size() || p_pop_vec.size() != color_vec.size()){
    std::cerr << "PlotGrace::plotPhenotype() Size of vector is wrong." << std::endl;
    return;
  }
  /* If the data is from catalyst, we output it on
     g0. Otherwise, we output it on g1. */
  unsigned int size = color_vec.size();
  for(unsigned int i=0;i<size;i++){
    if(if_catalyst_vec[i]){
      ::GracePrintf("g1.s%d point %d, %d",color_vec[i],time,p_pop_vec[i]);
      ::GracePrintf("g1.s%d point %d, %d",color_vec[i]+::par_color_size,time,m_pop_vec[i]);
    }else{
      ::GracePrintf("g2.s%d point %d, %d",color_vec[i],time,p_pop_vec[i]);
      ::GracePrintf("g2.s%d point %d, %d",color_vec[i]+::par_color_size,time,m_pop_vec[i]);
    }
  }
}

void PlotGrace::plotFunction(int time,int symmetric,int asymmetric_c,int asymmetric_t,int parasite,int undefined)
{
  int total = symmetric + asymmetric_c + asymmetric_t + parasite + undefined;
  ::GracePrintf("g0.s0 point %d, %d",time,total);
  ::GracePrintf("g0.s1 point %d, %d",time,symmetric);
  ::GracePrintf("g0.s2 point %d, %d",time,asymmetric_c);
  ::GracePrintf("g0.s3 point %d, %d",time,asymmetric_t);
  ::GracePrintf("g0.s4 point %d, %d",time,parasite);
  ::GracePrintf("g0.s5 point %d, %d",time,undefined);
}

void PlotGrace::save()
{
  if(::GraceIsOpen()){
    ::GraceFlush();

    /* Tell Grace to save the data */
    ::GracePrintf("saveall \"%s.agr\"",par_grace_outfile_name);
  }else{
    std::cerr << "PlotGrace::save() grace is not running." << std::endl;
  }
}

