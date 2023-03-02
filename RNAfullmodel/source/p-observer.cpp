#include "p-observer.hpp"
extern "C"{
#include "cash2.h"
#include "para.h"
}


Pobserver::Pobserver()
  : undefined_pop(0)
{
  /* Set the target structure */
  Pobserver::full_str2coarse_str(target_structure,par_target_structure);

  /*I don't know if these colours work at all*/
  /*Sandro's only colors... at least a bit more understandable*/
  int r=0,g=255,b=255, c=0;	//amount of each color RGB, c is counter
  double nr=250.;    //nr of ColorRGB's you want to create
  double range=1000.;  // range of coloursteps
  double x = range/nr;           
  int y=0;
  
  for(c=0;c<(int)range;c++){		/*you start from cyan*/
      if(c<255){
        b = b - 1;	/*first go green*/
      }else if(c<255*2){
        r = r + 1;	/*then yellow*/
      }else if(c<255*3){
        g = g - 1;	/*then to red*/
      }
      else if(c<255*4)
        b= b +1;		/*then magenta*/
      if(c == (int)(y*x+0.5)){
        color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(r,g,b),1+y));
//        std::cout <<"index: "<< y+1 << " colour: " << r <<","<< g <<","<< b <<"\n";
	y++;
      }
    }
  /*color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,0),250));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,255),251));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,255),252));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,0),253));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(128,128,128),254));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,255),255));
  */
  /*Nobuto's colors*/
  
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,0),250));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,255),251));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,51,102),252));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,51,102),253));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,204,204),254));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,204,153),255));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,204,51),256));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,0,51),257));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,153),258));		//1.2
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,51,0),259));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,255,204),260));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,153,153),261));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,51,153),262));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,153),263));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,255),264));		//1
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,102,0),265));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,102,204),266));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,0,204),267));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,51,51),268));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,0,0),269));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,0,51),270));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,0,204),271));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,51,0),272));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,0,0),273));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,153,153),274));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,204,255),275));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,0,102),276));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,255,153),277));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,51,204),278));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,255,51),279));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,102,51),280));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,102,0),281));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,51,102),282));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,255,102),283));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,102,0),284));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,204,204),285));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,204,153),286));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,102,102),287));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,153,255),288));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,153,102),289));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,255,0),290));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,102,204),291));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,153,153),292));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,0,153),293));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,153,102),294));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,204,204),295));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,51),296));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,102,153),297));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,102,102),298));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,153,204),299));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,102,255),300));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,51,204),301));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,153,204),302));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,153,204),303));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,102,102),304));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,204),305));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,51,51),306));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,255,102),307));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,102,51),308));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,153,153),309));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,102,51),310));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,51,153),311));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,51,51),312));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,153,102),313));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,51,204),314));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,153,153),315));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,0,255),316));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,0,204),317));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,153,51),318));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,102,255),319));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,0,153),320));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,102,0),321));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,51,153),322));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,51,153),323));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,255,102),324));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,51,0),325));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,153,0),326));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,153,204),327));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,204,0),328));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,102,51),329));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,153,51),330));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,51,0),331));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,204,51),332));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,102,0),333));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,51,153),334));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,153,0),335));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,204,0),336));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,204,51),337));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,51,102),338));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,153,102),339));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,0,51),340));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,204,102),341));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,102,102),342));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,102,204),343));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,204,102),344));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,255),345));		//6
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,0,51),346));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,255,153),347));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,204,255),348));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,51,255),349));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,51,204),350));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,0,0),351));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,255,153),352));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,204,153),353));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,153,51),354));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,51,204),355));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,102,153),356));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,0,102),357));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,204,0),358));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,51,255),359));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,51,255),360));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,204,204),361));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,204,102),362));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,153,204),363));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,255,204),364));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,153),365));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,102,255),366));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,51,51),367));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,102),368));		//1.3
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,0,0),369));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,153,255),370));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,153,102),371));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,51,153),372));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,153,0),373));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,204,102),374));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,102,255),375));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,255,204),376));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,102,255),377));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,255,51),378));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,204,255),379));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,51,51),380));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,102,51),381));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,102),382));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,204,255),383));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,204,153),384));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,204,0),385));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,204,51),386));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,102),387));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,153,102),388));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,255),389));		//5
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,153,255),390));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,102,102),391));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,0,102),392));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,153,51),393));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,51,51),394));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,204,0),395));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,204,153),396));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,204),397));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,0),398));		//4
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,102),399));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,102,153),400));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,204,153),401));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,153,0),402));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,0,153),403));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,51,204),404));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,51,102),405));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,0,51),406));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,153,51),407));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,255,0),408));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,204,51),409));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,204,204),410));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,102,153),411));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,153,0),412));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,0,255),413));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,255,255),414));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,102,204),415));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,102,102),416));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,0,255),417));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,102,51),418));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,153),419));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,204),420));		//1.1
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,102,204),421));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,51,0),422));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,153,255),423));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,153,204),424));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,153,0),425));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,0,204),426));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,102,204),427));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,153,153),428));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,0),429));		//2
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,51),430));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,255,102),431));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,204,102),432));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,0,102),433));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,204,204),434));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,255,0),435));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,153,255),436));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,51,255),437));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,0,255),438));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,102,255),439));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,204,255),440));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,204,255),441));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,51,255),442));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,204,0),443));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,153,255),444));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,255,204),445));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,0,153),446));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,255,204),447));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,255,255),448));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,255,0),449));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,102,0),450));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,51,0),451));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,255,153),452));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,51,102),453));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,255,255),454));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,0,51),455));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,153,51),456));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,255,255),457));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,255,51),458));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(204,102,153),459));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,255,51),460));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(0,204,102),461));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(153,204,51),462));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(102,102,153),463));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(255,255,0),464));		//3
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(51,51,255),465));
  color_map.insert(std::pair<const boost::tuple<int,int,int>,int>(boost::tuple<int,int,int>(128,128,128),466));
  
}

void Pobserver::insert(const std::string &seq_1,const std::string &str_1,const std::string &end3_1,const std::string &end5_1,unsigned int pop_1,const std::string &seq_2,const std::string &str_2,const std::string &end3_2,const std::string &end5_2,unsigned int pop_2)
{
  /* For the RNA 2ndary structure, we use coarse grained
     structure. */
  std::string costr_1,costr_2;
  Pobserver::full_str2coarse_str(costr_1,str_1);
  Pobserver::full_str2coarse_str(costr_2,str_2);

  /* We construct phenotype by correctly choosing the
     plus-phenotype and the minus-phenotype. */
  Phenotype phenotype;
  if(Pobserver::if1stIsPlus(costr_1,end3_1,end5_1,costr_2,end3_2,end5_2)){
    /* Since str_1 is plus-phenotype, set_p_str() takes
       costr_1 as the argument. */
    phenotype.set_p_str(costr_1);
    phenotype.set_p_end3(end3_1);
    phenotype.set_p_end5(end5_1);
    phenotype.set_m_str(costr_2);
    phenotype.set_m_end3(end3_2);
    phenotype.set_m_end5(end5_2);

    /* Try to insert */
    std::pair<tree_type::iterator,bool> result = tree.insert(std::pair<Phenotype,Pobserver::Population>(phenotype,Pobserver::Population()));

    /* If this is the first time to insert, set if_catalyst */
    if(result.second){
      result.first->second.set_p_if_catalyst(result.first->first.get_p_str()==target_structure && Pobserver::maxbasepair(result.first->first.get_p_end3())>par_m_threshold);
      result.first->second.set_m_if_catalyst(result.first->first.get_m_str()==target_structure && Pobserver::maxbasepair(result.first->first.get_m_end3())>par_m_threshold);
    }
    
    /* Update the Population attributes */
    result.first->second.set_p_pop(result.first->second.get_p_pop() + pop_1);
    result.first->second.set_m_pop(result.first->second.get_m_pop() + pop_2);
    if(result.first->second.get_p_master_pop() + result.first->second.get_m_master_pop() < pop_1+pop_2){
      result.first->second.set_p_master(seq_1);
      result.first->second.set_p_master_pop(pop_1);
      result.first->second.set_m_master(seq_2);
      result.first->second.set_m_master_pop(pop_2);
    }
  }else{
    phenotype.set_p_str(costr_2);
    phenotype.set_p_end3(end3_2);
    phenotype.set_p_end5(end5_2);
    phenotype.set_m_str(costr_1);
    phenotype.set_m_end3(end3_1);
    phenotype.set_m_end5(end5_1);

    std::pair<tree_type::iterator,bool> result = tree.insert(std::pair<Phenotype,Pobserver::Population>(phenotype,Pobserver::Population()));

    if(result.second){
      result.first->second.set_p_if_catalyst(result.first->first.get_p_str()==target_structure && Pobserver::maxbasepair(result.first->first.get_p_end3())>par_m_threshold);
      result.first->second.set_m_if_catalyst(result.first->first.get_m_str()==target_structure && Pobserver::maxbasepair(result.first->first.get_m_end3())>par_m_threshold);
    }

    result.first->second.set_p_pop(result.first->second.get_p_pop() + pop_2);
    result.first->second.set_m_pop(result.first->second.get_m_pop() + pop_1);
    if(result.first->second.get_p_master_pop() + result.first->second.get_m_master_pop() < pop_1+pop_2){
      result.first->second.set_p_master(seq_2);
      result.first->second.set_p_master_pop(pop_2);
      result.first->second.set_m_master(seq_1);
      result.first->second.set_m_master_pop(pop_1);
    }
  }
}


bool Pobserver::if1stIsPlus(const std::string &str_1,const std::string &end3_1,const std::string &end5_1,const std::string &str_2,const std::string &end3_2,const std::string &end5_2) const
{
  if(str_1 < str_2)
    return true;
  else if(str_1 > str_2)
    return false;

  if(end3_1 < end3_2)
    return true;
  else if(end3_1 > end3_2)
    return false;

  if(end5_1 < end5_2)
    return true;
  else if(end5_1 > end5_2)
    return false;

  return true;
}

void Pobserver::full_str2coarse_str(std::string &course_structure,const std::string &structure) const
{
  char str[structure.size()+1];
  std::strcpy(str,structure.c_str());

  char *b2c = ::b2C(str);
  char *expand_shapiro = ::expand_Shapiro(b2c);
  course_structure = expand_shapiro;
  std::free(b2c);
  std::free(expand_shapiro);

}

void Pobserver::getEverything()
{
  tree_type::const_iterator end_iter = tree.end();
  for(tree_type::const_iterator iter = tree.begin();iter != end_iter;iter++){
    std::cout << iter->first.get_p_str() << '\n';
    std::cout << iter->first.get_p_end3() << '\n';
    std::cout << iter->first.get_p_end5() << '\n';
    std::cout << iter->second.get_p_master() << '\n';
    std::cout << "total=" << iter->second.get_p_pop() << '\n';
    std::cout << "master=" << iter->second.get_p_master_pop() << '\n';

    std::cout << iter->first.get_m_str() << '\n';
    std::cout << iter->first.get_m_end3() << '\n';
    std::cout << iter->first.get_m_end5() << '\n';
    std::cout << iter->second.get_m_master() << '\n';
    std::cout << "total=" << iter->second.get_m_pop() << '\n';
    std::cout << "master=" << iter->second.get_m_master_pop() << '\n';
    std::cout << "-------------------------------------------\n" ;
  }
}

void Pobserver::setAllPopZero()
{
  tree_type::const_iterator end_iter = tree.end();
  for(tree_type::iterator iter = tree.begin();iter != end_iter;iter++){
    iter->second.set_p_pop(0);
    iter->second.set_p_master("undetermined");
    iter->second.set_p_master_pop(0);
    iter->second.set_m_pop(0);
    iter->second.set_m_master("undetermined");
    iter->second.set_m_master_pop(0);
  }

  undefined_pop = 0;
}
void Pobserver::makeRanking()
{
  /* We make an vector of iterator to the map elements. Then
     we sort the vector according to the population size of
     phenotype. */
  tree_iter_vec.clear();
  /* We delete the phenotype with zero population, and if
     those phenotypes have any color other than Color 1 (i.e.,
     the color of undefined phenotype), we return that color
     to color_manager'. */
  tree_type::iterator iter = tree.begin();
  tree_type::const_iterator end_iter = tree.end();
  tree_type::iterator prev_iter;
  while(iter != end_iter){
    if(iter->second.get_p_pop()+iter->second.get_m_pop() == 0){
      if(iter->second.get_color()!=1){
	color_manager.returnColor(iter->second.get_color());
      }
      prev_iter = iter;
      iter++;
      tree.erase(prev_iter);
    }else{
      /* Only surviving phenotypes are in the vector. */
      tree_iter_vec.push_back(iter);
      iter++;
    }
  }
  
  std::sort(tree_iter_vec.begin(),tree_iter_vec.end(),Pobserver::LartherThanByPop());

  /* For top 'par_n_phenotype_cat' catalyst and
     'par_n_phenotype_par' parasite, we put a color if they
     haven't gotten one. */
  int ncat=0,npar=0;
  for(std::vector<tree_type::iterator>::iterator iter=tree_iter_vec.begin();iter!=tree_iter_vec.end(); iter++){
    /* Color 1 is the color of undefined phenotype. */
    if((*iter)->second.get_p_if_catalyst()||(*iter)->second.get_m_if_catalyst()){
      if(ncat < ::par_n_phenotype_color_cat){
	ncat++;
	if((*iter)->second.get_color()==::par_color_undefined){
	  (*iter)->second.set_color(color_manager.getColor());
	}
      }
    }else{
      if(npar < ::par_n_phenotype_color_par){
	npar++;
	if((*iter)->second.get_color()==::par_color_undefined){
	  (*iter)->second.set_color(color_manager.getColor());
	}
      }
    }
    if(ncat >= ::par_n_phenotype_color_cat && npar >= ::par_n_phenotype_color_par)
      break;
  }
}

void Pobserver::getPopulation(std::vector<int> &functional_pop,std::vector<unsigned int> &color_vec,std::vector<int> &p_population_vec,std::vector<int> &m_population_vec,std::vector<bool> &if_catalyst_vec) const
{
  /* functional_pop will get the population of [0] symmetric
     catalysts, [1] asymmetric catalysts with function, [2]
     asymmetric catalysts without function, [3]
     parasites, and [4] undefined population.*/
  functional_pop.assign(std::vector<int>::size_type(5),int(0));
  functional_pop[4] = Pobserver::undefined_pop;

  color_vec.clear();
  p_population_vec.clear();
  m_population_vec.clear();
  if_catalyst_vec.clear();

  /* Count the population and set it to the 'population'
     vector. 'Color_vec' will tell the color of the
     phenotype. The first two elements of the vectors are for
     uncolored phenotype. Thus, they are special. The firrst
     element is for uncolored calasyst phenotype, and the
     second one is for uncolored parasite phenotype. */
  color_vec.push_back(::par_color_undefined);
  p_population_vec.push_back(0);
  m_population_vec.push_back(0);
  if_catalyst_vec.push_back(true);

  color_vec.push_back(::par_color_undefined);
  p_population_vec.push_back(0);
  m_population_vec.push_back(0);
  if_catalyst_vec.push_back(false);

  tree_type::const_iterator end_iter = tree.end();
  for(tree_type::const_iterator iter = tree.begin();iter != end_iter;iter++){

    /* Count up the population in 4 categories. */
    if(iter->second.get_p_if_catalyst()&&iter->second.get_m_if_catalyst()){
      functional_pop[0] += iter->second.get_p_pop() + iter->second.get_m_pop();
    }else if(iter->second.get_p_if_catalyst()||iter->second.get_m_if_catalyst()){
      if(iter->second.get_p_if_catalyst()){
	functional_pop[1] += iter->second.get_p_pop();
	functional_pop[2] += iter->second.get_m_pop();
      }else{
	functional_pop[1] += iter->second.get_m_pop();
	functional_pop[2] += iter->second.get_p_pop();
      }
    }else{
      functional_pop[3] += iter->second.get_p_pop() + iter->second.get_m_pop();
    }

    /* Count up the population of phenotypes clasified by
       their color. */
    if(iter->second.get_color()!=::par_color_undefined){
      color_vec.push_back(iter->second.get_color());
      p_population_vec.push_back(iter->second.get_p_pop());
      m_population_vec.push_back(iter->second.get_m_pop());
      if_catalyst_vec.push_back((iter->second.get_p_if_catalyst()||iter->second.get_m_if_catalyst()));
    }else{
      /* Thease are for uncolored (color=1) phenotypes. */
      if(iter->second.get_p_if_catalyst()||iter->second.get_m_if_catalyst()){
	p_population_vec[0] += iter->second.get_p_pop();
	m_population_vec[0] += iter->second.get_m_pop();
      }else{
	p_population_vec[1] += iter->second.get_p_pop();
	m_population_vec[1] += iter->second.get_m_pop();
      }
    }
  }  
}

int Pobserver::maxbasepair(const std::string &end3)const
{
  int max=0;
  std::string::const_iterator end_iter = end3.end();
  for(std::string::const_iterator iter=end3.begin();iter!=end_iter;iter++){
    switch(*iter) {
    case 'A': case 'U': max+=2; break;
    case 'G': case 'C': max+=3; break;
    }
  }
  return max;
}

void Pobserver::outputSequences(unsigned int time) const
{
  std::ofstream fout(par_sequence_file_name,std::ios::app);

  if(fout.fail()){
    std::cerr << "Pobserver::outputSequences() Cannot open a file " << par_sequence_file_name << std::endl;
    std::exit(-1);
  }

  int n_parasites=0,n_catalysts=0;
  std::vector<tree_type::iterator>::const_iterator end_iter=tree_iter_vec.end();
  for(std::vector<tree_type::iterator>::const_iterator iter=tree_iter_vec.begin();iter!=end_iter;iter++){
    if((*iter)->second.get_p_if_catalyst()&&(*iter)->second.get_m_if_catalyst()){
      /* If enough sequences are output, we don't output
	 anymore. */
      if(n_catalysts < par_n_cata_output_seq){
	fout << "t" << time << " s" << n_catalysts << "+ " << (*iter)->second.get_p_master() <<" p"<<(*iter)->second.get_p_pop() << " c" << (*iter)->second.get_color() << " " << (*iter)->first.get_p_str() << "\n";
	fout << "t" << time << " s" << n_catalysts << "- " << (*iter)->second.get_m_master() <<" p"<<(*iter)->second.get_m_pop() << " c" << (*iter)->second.get_color() << " " << (*iter)->first.get_m_str() << "\n";

	n_catalysts++;
      }
    }else if((*iter)->second.get_p_if_catalyst()||(*iter)->second.get_m_if_catalyst()){
      if(n_catalysts < par_n_cata_output_seq){
	char p_c = ((*iter)->second.get_p_if_catalyst()) ? 'c' : 't';
	char m_c = ((*iter)->second.get_m_if_catalyst()) ? 'c' : 't';

	fout <<"t"<<time<<" "<<p_c<<n_catalysts<<"+ "<<(*iter)->second.get_p_master()<<" p"<<(*iter)->second.get_p_pop()<< " c" << (*iter)->second.get_color() <<" "<<(*iter)->first.get_p_str()<<"\n";
	fout <<"t"<<time<<" "<<m_c<<n_catalysts<<"- "<<(*iter)->second.get_m_master()<<" p"<<(*iter)->second.get_m_pop()<< " c" << (*iter)->second.get_color() <<" "<<(*iter)->first.get_m_str() << "\n";

	n_catalysts++;
      }
    }else{
      if(n_parasites < par_n_para_output_seq){
	fout << "t" << time << " p" << n_parasites << "+ " << (*iter)->second.get_p_master()<<" p"<<(*iter)->second.get_p_pop()<< " c" << (*iter)->second.get_color() <<" "<<(*iter)->first.get_p_str() << "\n";
	fout << "t" << time << " p" << n_parasites << "- " << (*iter)->second.get_m_master()<<" p"<<(*iter)->second.get_m_pop()<< " c" << (*iter)->second.get_color() <<" "<<(*iter)->first.get_m_str() << "\n";
	
	n_parasites++;
      }
    }
    
    /* If we output all, we stop the for-loop. */
    if(n_parasites >= par_n_para_output_seq && n_catalysts >= par_n_cata_output_seq)
      break;
  }
}

/*
  This method returns a color of a molecule by its function
  (catalyst or parasite) and the AUGC-composition of its
  terminal sequence.
*/
void Pobserver::getColorByComposition(int i,int j,unsigned int &color_func,unsigned int &color_comp,const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &c_structure,struct point ***neighbor, const TYPE2 **world) const
{
  /*In this version colors according to HD of neighbourhood, including focal molecule
  EXCEPT IF YOU'RE PARASITE, IN THAT CASE YOU GET COLOURED BLUE, this will be taken care of  later
  
  In this version also left picture (color_func) is done on the neighbourhood, but ignores empty neighbours
  also, color coding is simplified, as junk, one-strand only cataysts and rest are all grey*/
  
  int k,neirow,neicol,hd,c_hd,countarray[MAXLEN+2]={0},max_freq=-1;
  
  for(k=0;k<9;k++){
    //Here, for each seq we check HD from C-cat or complem and take the smallest,
    //remember that sequence may not exist, i.e. it's empty space so you 
    //first check if it exists and than iterate only on those that are there.
    //At the end of the cycle we have to know how many and what's the hd of each
    
    neirow=neighbor[k][i][j].row;	//first get position
    neicol=neighbor[k][i][j].col;
    
    if(world[neirow][neicol].seq[0] != 'N'){
      hd=0;
      c_hd=0;
      for(unsigned int l=0;world[neirow][neicol].seq[l] != '\0';l++){
	if(world[neirow][neicol].seq[l] != Ccat[l]) hd++;
        if(world[neirow][neicol].seq[l] != c_Ccat[l]) c_hd++;
      }
      hd = (hd < c_hd)? hd : c_hd;		/*assign to hd the smallest btwn hd and c_hd*/
    }
    else hd=MAXLEN+1;
    countarray[hd]++;				//take array as long as the seq, and put at corresp position how many times we got that hd, 
    						//ignore that two values can be as frequent.
  }
    
  for(k=0;k<MAXLEN+2;k++){
    if(countarray[k]>max_freq) {max_freq=countarray[k];hd=k;}
  }
  if(hd<=7) color_comp=35*hd+1;
  else if(hd>MAXLEN) color_comp=0;
  else color_comp=getColorIndex(255,255,255);
  /* 
     The color by function. According to the type of sequence,
     the color coding is different, and moreover which is
     terminal used for color coding is also different. Thus,
     we first need to know which type the sequence is and set
     the terminal that is going to be used to determine the
     color. --Sensei

  */
  /*Here for each neighbour we save only the final color, no need to refold (as was in previous versions), 
  as we already have 'who'. Plus, we coarse grain phenotypes further (also, in case of hybrid phenotypes, 
  e.g. 'c', we take into account wether it's the catalytic strand or not):
  
    getColorIndex(0,255,255);	//C			Ccat (if available)	cyan
    getColorIndex(255,255,255);	//u			unit of replication,	white
    getColorIndex(0,255,0);	//h,o,a,l,b_u,c_h	extended helpers	green
    getColorIndex(255,0,0);	//s,t,b_s,c_s		extended stallers	red
    getColorIndex(255,255,0);	//p,e			parasites		yellow
    getColorIndex(128,128,128);	//j			junk			grey
    getColorIndex(0,0,255);	//v			extended catalysts, 	blue
  
  */
  
  int colors[9]={-1};
  for(k=0;k<9;k++){
    
    neirow=neighbor[k][i][j].row;	//first get position
    neicol=neighbor[k][i][j].col;
        
    if(world[neirow][neicol].seq[0]=='N') colors[k]=0;
    else{
      switch (world[neirow][neicol].who){
        case 'C': colors[k] = getColorIndex(0,255,255); break;
	case 'u': colors[k] = getColorIndex(255,255,255); break;
	case 'h': 
	case 'o': 
	case 'a': 
	case 'l': colors[k] = getColorIndex(0,255,0); break;
	case 'b': if(world[neirow][neicol].kcat==1.) colors[k] = getColorIndex(0,255,0);
		  else colors[k] = getColorIndex(255,0,0);
		  break;
	case 'c': if(world[neirow][neicol].kcat==1.) colors[k] = getColorIndex(0,255,0);
		  else colors[k] = getColorIndex(255,0,0);
		  break;
	case 's':
	case 't': colors[k] = getColorIndex(255,0,0);
		  break;
	case 'p': 
	case 'e': colors[k] = getColorIndex(255,255,0);
		  if(k==0) color_comp=getColorIndex(0,0,255);
		  break;
	case 'j': colors[k] = getColorIndex(128,128,128);
		  break;
	case 'v': colors[k] = getColorIndex(0,0,255);
		  break;
	default: fprintf(stderr, "Error in getColorByComposition(): couldn't recognize world[neirow][neicol].who.\n");
      }
    }
  }
  //Now, we got array that says what color is each neighbour, we find the most frequent colour

  //also consider that you have to sistematically ignore zeroes
  //since numbers may get big, can't use same as before, we first sort the array and then find mode
  int elements = sizeof(colors) / sizeof(colors[0]); 
  std::sort(colors, colors + elements);  
  
  int current_col=colors[0], count=0, hicol=colors[0], hifreq=0;
  for(k=0;k<9;k++){
    if(colors[k]==0) continue;
    if(current_col==colors[k]){
      count++;
    }
    else{
      if(count>hifreq){
        hicol=current_col;
	hifreq=count;
      }
      current_col=colors[k];
      count=1;
    }
  }
  if(count>hifreq){
    hicol=current_col;
    hifreq=count;
  }
  color_func=hicol;
  
//  for(k=0;k<9;k++) std::cout<<colors[k]<<" ";
//  std::cout <<'\n'<<hicol<<" "<<hifreq<<'\n';
}
