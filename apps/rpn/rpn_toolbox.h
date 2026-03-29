#ifndef RPN_TOOLBOX_H
#define RPN_TOOLBOX_H

//#include <escher.h>
#include <apps/i18n.h>
//#include <apps/math_toolbox.h>
#include "shared/math_toolbox_controller.h"

namespace Rpn {

class InputController;
class StackController;

class Toolbox final : public Shared::MathToolboxController {
public:
  Toolbox(InputController * inputController, StackController * stackController);
protected:
  bool selectLeaf(int selectedRow);
private:
  InputController * m_inputController;
  StackController * m_stackController;
};

}

#endif
