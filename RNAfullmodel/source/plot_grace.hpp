#include <vector>

#ifndef PLOTGRACE
#define PLOTGRACE
class PlotGrace {
 public:
  PlotGrace();
  ~PlotGrace();

  void plotPhenotype(int time,const std::vector<unsigned int> &color_vec,const std::vector<int> &p_pop_vec,const std::vector<int> &m_pop_vec,const std::vector<bool> &if_catalyst_vec);
  void plotFunction(int time,int symmetric,int asymmetric_c,int asymmetric_t,int parasite,int undefined);
  void save();
};
#endif // PLOTGRACE
