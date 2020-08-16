////////////////////////////////////////////////////////////
#include "../basegame/basegame.hpp"
#include "mantissa.cl.hpp"
#include "../consi/logger.hpp"
#include "../shing/consolearg.hpp"
#include "../roengl/neocommando.hpp"
#include "hook.cl.hpp"
#include "hook.ceeferinc.cl.hpp"
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
namespace Mantissa{
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
   BG::Starter starter([]{
      using namespace Mantissa;
      auto& bg = BG::bg();
      BG::no3d(); // screw that aye
      BG::disableDither();
      manti();
      BG::touchhandlers / [](const TouchInfo& tinfoil){
         return manti->handleTouchEvent(tinfoil);
      };
      BG::drawer2D / []{
         manti->drawPost2D();
      };
      BG::updaters / []{
         manti->update();
      };
      BG::postparticlers / []{
      };
      Roen::neoc("system") / []{
         Roen::neocruler();
         Roen::neoc("hello") / []{
            CALLconslog(hello, uist() / 196_xcoel / "hello was called");
         };
         Roen::neoc("sprites") / []{
         };
         BG::marsiso().enableNeocMenu();
      };
   });
////////////////////////////////////////////////////////////
}; // end namespace Mantissa
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
