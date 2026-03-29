#include "app.h"
#include "rpn_content_view.h"
#include "rpn_stack_controller.h"
#include "rpn_input_controller.h"
#include "../shared/poincare_helpers.h"
#include <assert.h>
//#include <poincare_nodes.h>

using namespace Poincare;

namespace Rpn {

StackController::StackController(Responder * parentResponder, Stack * stack, InputController * inputController, ContentView * view, Poincare::Context *context) :
  ViewController(parentResponder),
  m_stack(stack),
  m_inputController(inputController),
  m_view(view),
  m_context(context)
{

}

Escher::View * StackController::view() {
  return m_view;
}
void StackController::viewWillAppear() {
    printf("viewWillAppear\n");
    ViewController::viewWillAppear();
    reloadAndScroll();
}

void StackController::handleResponderChainEvent(Escher::Responder::ResponderChainEvent event) {
    if (ResponderChainEventType::HasBecomeFirst == event.type) {
        printf("selectRowxxxxxxxx\n");
        selectRow(m_stack->length() - 1);
        App::app()->setFirstResponder(stackView());

        //reloadAndScroll();
        //printf("selectCellAtLocation 0\n");
        //stackView()->selectCellAtLocation(0, m_stack->length() - 1);
    }
}


bool StackController::handleEvent(Ion::Events::Event event) {
  size_t stackRow = m_stack->length() - selectedRow() - 1;
  bool handled = true;

  if (event == Ion::Events::Down || event == Ion::Events::Back) {
    stackView()->deselectTable();
      App::app()->setFirstResponder(m_inputController);
  }
  else if (event == Ion::Events::OK || event == Ion::Events::EXE) {
    m_inputController->setText((*m_stack)[stackRow]);
    App::app()->setFirstResponder(m_inputController);
    reloadAndScroll();
    stackView()->deselectTable();
  }
  else if (event == Ion::Events::Backspace) {
    m_stack->dropNth(stackRow);
    stackView()->reloadData(false);
    if (stackRow == 0) {
      stackView()->deselectTable();
        printf("selectCellAtLocation 1\n");
      stackView()->selectCellAtLocation(0, m_stack->length() - 1);
    }
  }
  else if (event == Ion::Events::Clear) {
    while (m_stack->length() > stackRow+1) {
      (*m_stack)(Stack::POP);
    }
    reloadAndScroll();
  }
  else {
    handled = false;
  }

  return handled;
}

KDCoordinate StackController::nonMemoizedRowHeight(int i) {
    printf("nonMemoizedRowHeight %d %d\n", i, m_stack->height(m_stack->length() - i - 1) + k_padding * 2);
  return m_stack->height(m_stack->length() - i - 1) + k_padding * 2;
}

void StackController::fillCellForRow(Escher::HighlightCell * cell, int index) {
    printf("fillCellForRow index%d\n", index);
  Escher::EvenOddExpressionCell *realCell = static_cast<Escher::EvenOddExpressionCell *>(cell);
  realCell->setEven(index%2);
  realCell->setAlignment(1, 0.5);

  realCell->setLayout(createLayout(index));
  realCell->reloadCell();
}

Escher::HighlightCell * StackController::reusableCell(int index, int type) {
    printf("reusableCell index %d type %d\n", index, type);
    return &m_cells[index];
}
Poincare::Layout StackController::createLayout(int index) {
  Expression e = Expression::Parse((*m_stack)[m_stack->length() - index - 1], nullptr);
  return e.createLayout(MathPreferences::SharedPreferences()->displayMode(), MathPreferences::SharedPreferences()->numberOfSignificantDigits(), m_context);
}

void StackController::reloadAndScroll(int index) {
    printf("reloadAndScroll index %d m_stack->length() %d\n", index, m_stack->length());
  if (index < 0 || index >= static_cast<int>(m_stack->length())) {
    index = m_stack->length()> 0 ? m_stack->length() - 1 : 0;
  }
    selectRow(index);
  stackView()->reloadData(false);
  stackView()->scrollToCell(0, index);//m_stack->length()> 0 ? m_stack->length() - 1 : 0);

}

Escher::SelectableTableView* StackController::stackView() {
  return m_view->stackView();
}


I18n::Message StackController::operator()(const char* text) {
  auto r = (*m_stack)(text, m_context);
  if (r != I18n::Message::Default) {
    reloadAndScroll();
  }
  return r;
}

I18n::Message StackController::operator()(Stack::StackOperation op) {
  auto r = (*m_stack)(op);
  if (r == I18n::Message::Default) {
    reloadAndScroll();
  }
  return r;
}

I18n::Message StackController::operator()(Stack::SpecialOperation op) {
  auto r = (*m_stack)(op, m_context);
  if (r == I18n::Message::Default) {
    reloadAndScroll();
  }
  return r;
}

I18n::Message StackController::operator()(Poincare::Internal::Type op) {
  auto r = (*m_stack)(op, m_context);
  if (r == I18n::Message::Default) {
    reloadAndScroll();
  }
  return r;
}

I18n::Message StackController::operator()(I18n::Message op) {
  auto r = (*m_stack)(op, m_context);
  if (r == I18n::Message::Default) {
    reloadAndScroll();
  }
  return r;
}

}
