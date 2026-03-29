#include "rpn_toolbox.h"

#include "rpn_input_controller.h"
#include "rpn_stack_controller.h"
#include "app.h"
#include "escher/toolbox_message_tree.h"
namespace Rpn {

Toolbox::Toolbox(InputController * inputController, StackController * stackController) :
  m_inputController(inputController),
  m_stackController(stackController)
{
}

bool Toolbox::selectLeaf(int selectedRow) {
  Escher::ToolboxMessageTree * messageTree = (Escher::ToolboxMessageTree *)m_messageTreeModel->childAtIndex(selectedRow);
  //TODO
  //m_selectableTableView.deselectTable();

  I18n::Message r = I18n::Message::Default;
  if (m_inputController->pushInput()) {
    r = (*m_stackController)(messageTree->label());
  }
  //App::app()->dismissModalViewController();
  if (r != I18n::Message::Default) {
      App::app()->displayWarning(r);
  }
  return true;
}

}
