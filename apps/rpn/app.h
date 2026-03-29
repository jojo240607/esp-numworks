#ifndef RPN_APP_H
#define RPN_APP_H

#include "../shared/shared_app.h"
#include "rpn_content_view.h"
#include "rpn_stack.h"
#include "rpn_stack_controller.h"
#include "rpn_input_controller.h"
#include "rpn_toolbox.h"

namespace Rpn {

class App : public Shared::SharedApp {
public:
    class Descriptor : public Escher::App::Descriptor {
    public:
        I18n::Message name() const override;
        I18n::Message upperName() const override;
        const Escher::Image* icon() const override;
    };
  class Snapshot : public Shared::SharedApp::Snapshot {
  public:
    Snapshot();
    App * unpack(Escher::Container * container) override;
    void reset() override;
    const Descriptor * descriptor() const override;
    Stack * stack();
  private:
    Stack m_stack;
  };
private:
  App(Snapshot * snapshot);
  //::Toolbox * toolboxForInputEventHandler(InputEventHandler * textInput) override;
  Toolbox * toolbox() override;
  StackController m_stackController;
  InputController m_inputController;
  ContentView m_view;
  Toolbox m_toolbox;
};

}

#endif
