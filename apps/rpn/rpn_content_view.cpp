#include "rpn_content_view.h"
#include "rpn_input_controller.h"
#include "rpn_stack_controller.h"

namespace Rpn {

ContentView::ContentView(Escher::Responder * parentResponder, InputController * inputController, StackController * stackController) :
  View(),
  m_stackView(parentResponder, stackController, stackController, stackController),
  m_inputView(parentResponder, m_textBuffer, sizeof(m_textBuffer), inputController, k_inputFormat),
  m_textBuffer("")
{
}

ContentView::~ContentView() {
}

Escher::View * ContentView::subviewAtIndex(int index) {
  assert(index >= 0 && index < numberOfSubviews());
  switch (index) {
    case 0:  return &m_stackView;
    default: return &m_inputView;
  }
}

void ContentView::layoutSubviews(bool force) {
  KDCoordinate inputViewFrameHeight = 38;
  KDRect mainViewFrame(0, 0, bounds().width(), bounds().height() - inputViewFrameHeight);
  //m_stackView.setFrame(mainViewFrame, force);

  setChildFrame(&m_stackView, mainViewFrame, force);

  KDRect inputViewFrame(0, bounds().height() - inputViewFrameHeight, bounds().width(), inputViewFrameHeight);
  //m_inputView.setLeftMargin(5);
    //m_inputView.setMargins();
  //m_inputView.setFrame(inputViewFrame, force);
  setChildFrame(&m_inputView, inputViewFrame, force);
}

void ContentView::reload() {
  layoutSubviews();
  markRectAsDirty(bounds());
}

}
