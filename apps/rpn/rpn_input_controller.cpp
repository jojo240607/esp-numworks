#include "rpn_input_controller.h"
#include "app.h"
#include <assert.h>

using namespace Poincare;

namespace Rpn {

InputController::InputController(Responder * parentResponder, Stack * stack, StackController * stackController, ContentView * view) :
  ViewController(parentResponder),
  m_stack(stack),
  m_stackController(stackController),
  m_view(view)
{
}

Escher::View * InputController::view() {
  return m_view;
}
void InputController::handleResponderChainEvent(Escher::Responder::ResponderChainEvent event) {
    if (ResponderChainEventType::HasBecomeFirst == event.type) {
        App::app()->setFirstResponder(inputView());
        inputView()->setEditing(true);
        m_stackController->reloadAndScroll();
    }
}


bool InputController::handleEvent(Ion::Events::Event event) {
  return false;
}

bool InputController::textFieldShouldFinishEditing(Escher::AbstractTextField * view, Ion::Events::Event event) {
  return event == Ion::Events::EXE || event == Ion::Events::OK;
}

bool InputController::textFieldDidReceiveEvent(Escher::AbstractTextField * view, Ion::Events::Event event) {
  if (handleEventSpecial(event, view) || handleEventOperation(event, view)) {
    m_stackController->reloadAndScroll();
    return true;
  }

  return false;
}

bool InputController::textFieldDidFinishEditing(Escher::AbstractTextField * view, Ion::Events::Event event) {
  if (pushInput()) {
    m_stackController->reloadAndScroll();
    return true;
  }
  else {
    return false;
  }
}

void InputController::textFieldDidAbortEditing(Escher::AbstractTextField * view) {
  m_stackController->reloadAndScroll();
}

void InputController::textFieldDidHandleEvent(Escher::AbstractTextField * view) {
  //return returnValue;
}

bool InputController::handleEventSpecial(Ion::Events::Event event, Escher::AbstractTextField * view) {
  const char *text = view->text();
  bool handled = true;
  bool clearInput = true;
  I18n::Message r = I18n::Message::Default;

  if (event == Ion::Events::Up && !m_stackController->empty()) {
    view->setEditing(false);
    App::app()->setFirstResponder(m_stackController);
  }
  else if (event == Ion::Events::XNT) {
    view->handleEventWithText("x");
    clearInput = false;
  }
  else if (event == Ion::Events::Backspace && *text == '\0') {
    (*m_stackController)(Stack::POP);
  }
  else if (event == Ion::Events::Clear) {
    if (view->isEditing() && *text) {
      view->setText("");
    }
    else {
      (*m_stackController)(Stack::CLEAR);
    }
  }
  else if (event == Ion::Events::Equal) {
    m_stackController->setApproximate(!m_stackController->approximate());
  }
  else if (event == Ion::Events::RightParenthesis) {
    (*m_stackController)(Stack::SWAP);
  }
  else if (event == Ion::Events::LeftParenthesis) {
    (*m_stackController)(Stack::ROT);
  }
  else if (event == Ion::Events::Space) {
    view->handleEventWithText("-");
    clearInput = false;
  }
  else if (event == Ion::Events::Ans) {
    r = (*m_stackController)(Stack::OVER);
  }
  else if ((event == Ion::Events::EXE || event == Ion::Events::OK) && *text == '\0') {
    r = (*m_stackController)(Stack::DUP);
  }
  else {
    handled = false;
  }

  if (r != I18n::Message::Default) {
      App::app()->displayWarning(r);
  }
  else if (handled && clearInput) {
    inputView()->setText("");
  }

  return handled;
}

struct Event2Type {
  Ion::Events::Event event;
  Poincare::Internal::Type op;
} ;

constexpr static Event2Type events2types[] {
  { Ion::Events::Plus, Poincare::Internal::Type::Add },
  { Ion::Events::Minus, Poincare::Internal::Type::Sub },
  { Ion::Events::Multiplication, Poincare::Internal::Type::Mult },
  { Ion::Events::Division, Poincare::Internal::Type::Div },

  { Ion::Events::Ln, Poincare::Internal::Type::Ln },
  { Ion::Events::Power, Poincare::Internal::Type::Pow },
  { Ion::Events::Sto, Poincare::Internal::Type::Store },

  { Ion::Events::Sine, Poincare::Internal::Type::Sin },
  { Ion::Events::Cosine, Poincare::Internal::Type::Cos },
  { Ion::Events::Tangent, Poincare::Internal::Type::Tan },
  { Ion::Events::Arcsine, Poincare::Internal::Type::ASin },
  { Ion::Events::Arccosine, Poincare::Internal::Type::ACos },
  { Ion::Events::Arctangent, Poincare::Internal::Type::ATan },

  { Ion::Events::Sqrt, Poincare::Internal::Type::Sqrt },
} ;

struct Event2Special {
  Ion::Events::Event event;
  Stack::SpecialOperation op;
} ;

constexpr static Event2Special events2specials[] {
  { Ion::Events::Exp, Stack::Exp },
  { Ion::Events::Log, Stack::CommonLogarithm },
  { Ion::Events::Square, Stack::Square },
} ;

bool InputController::handleEventOperation(Ion::Events::Event event, Escher::AbstractTextField * view) {
  for (size_t i = 0; i < sizeof(events2types)/sizeof(events2types[0]); i++) {
    if (events2types[i].event == event) {
      if (pushInput()) {
        auto r = (*m_stackController)(events2types[i].op);
        if (r != I18n::Message::Default) {
          App::app()->displayWarning(r);
        }
      }
      return true;
    }
  }

  for (size_t i = 0; i < sizeof(events2specials)/sizeof(events2specials[0]); i++) {
    if (events2specials[i].event == event) {
      if (pushInput()) {
        auto r = (*m_stackController)(events2specials[i].op);
        if (r != I18n::Message::Default) {
          App::app()->displayWarning(r);
        }
      }
      return true;
    }
  }

  return false;
}

bool InputController::pushInput() {
  const char *text = inputView()->draftText();

  if (*text == '\0') {
    return true;
  }

  auto r = (*m_stackController)(text);
  if (r == I18n::Message::Default) {
    inputView()->setText("");
    return true;
  }
  else {
    App::app()->displayWarning(r);
    return false;
  }
}

void InputController::setText(const char *text) {
  inputView()->setEditing(true);
  inputView()->setText(text);
}

Escher::AbstractTextField* InputController::inputView() {
  return m_view->inputView();
}

}
