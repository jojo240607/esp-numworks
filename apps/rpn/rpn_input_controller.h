#ifndef RPN_INPUT_CONTROLLER_H
#define RPN_INPUT_CONTROLLER_H

#include "rpn_stack_controller.h"
#include "rpn_stack.h"
#include "escher/view.h"
#include "ion/events.h"
#include "escher/view_controller.h"
#include "escher/responder.h"
#include "escher/text_field_delegate.h"
#include "escher/text_field.h"


namespace Rpn {
class ContentView;
class StackController;
//, public Escher::InputEventHandlerDelegate
class InputController : public Escher::ViewController, public Escher::TextFieldDelegate {
public:
  InputController(Escher::Responder * parentResponder, Stack * stack, StackController * stackController, ContentView * view);
  Escher::View * view() override;
  bool handleEvent(Ion::Events::Event event) override;
  //void didBecomeFirstResponder() override;
    void handleResponderChainEvent(Escher::Responder::ResponderChainEvent event) override;
  /* TextFieldDelegate */
  bool textFieldShouldFinishEditing(Escher::AbstractTextField * textField, Ion::Events::Event event) override;
  bool textFieldDidReceiveEvent(Escher::AbstractTextField * textField, Ion::Events::Event event) override;
  bool textFieldDidFinishEditing(Escher::AbstractTextField * textField, Ion::Events::Event event) override;
  void textFieldDidAbortEditing(Escher::AbstractTextField * textField) override;
  void textFieldDidHandleEvent(Escher::AbstractTextField * textField) override;

  bool pushInput();
  void setText(const char *text);

public:
  bool handleEventOperation(Ion::Events::Event event, Escher::AbstractTextField * view);
  bool handleEventSpecial(Ion::Events::Event event, Escher::AbstractTextField * view);
    Escher::AbstractTextField* inputView();

private:
  Stack *m_stack;
  StackController *m_stackController;
  ContentView *m_view;
};

}

#endif
