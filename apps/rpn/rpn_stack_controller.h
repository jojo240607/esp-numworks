#ifndef RPN_STACK_CONTROLLER_H
#define RPN_STACK_CONTROLLER_H

#include "rpn_stack.h"
#include "escher/list_view_data_source.h"
#include "escher/selectable_list_view_data_source.h"
#include "escher/even_odd_expression_cell.h"
#include "escher/view_controller.h"
#include "escher/view.h"
#include "escher/selectable_table_view.h"
#include "escher/selectable_table_view_delegate.h"
namespace Rpn {
  class ContentView;
  class InputController;

class StackController :
        public Escher::ViewController,
        public Escher::ListViewDataSource,
        public Escher::SelectableTableViewDelegate,
        public Escher::SelectableTableViewDataSource {
public:
  StackController(Responder * parentResponder, Stack * stack, InputController * inputController, ContentView * view, Poincare::Context *context);
  Escher::View * view() override;
  void handleResponderChainEvent(Escher::Responder::ResponderChainEvent event) override;
  bool handleEvent(Ion::Events::Event event) override;
    void viewWillAppear() override;
  int numberOfRows() const override {
      printf("StackController numberOfRows %d\n", m_stack->length());
      return m_stack->length();
  }
  //KDCoordinate rowHeight(int i) override;
  KDCoordinate nonMemoizedRowHeight(int row) override;
  Escher::HighlightCell * reusableCell(int index, int type) override;// { return &m_cells[index]; }
  int reusableCellCount(int type) { return Stack::k_stackSize; };
  virtual int typeAtLocation(int i, int j) { return 0; }
  //void willDisplayCellForIndex(Escher::HighlightCell * cell, int index) override;

  void fillCellForRow(Escher::HighlightCell *cell, int row) override;
  const char* operator[](size_t idx) const { return (*m_stack)[idx]; }
  Stack& stack() { return *m_stack; }
  void erase(bool upTo);

  bool approximate() { return m_stack->approximate; }
  void setApproximate(bool approx) { m_stack->approximate = approx; }
  void reloadAndScroll(int index = -1);

  bool empty() { return m_stack->empty(); }

  I18n::Message operator()(const char* text);
  I18n::Message operator()(Stack::StackOperation op);
  I18n::Message operator()(Stack::SpecialOperation op);
  I18n::Message operator()(Poincare::Internal::Type op);
  I18n::Message operator()(I18n::Message op);

  Escher::SelectableTableView* stackView();

  constexpr static int k_padding = 4;
private:
  bool isFull();
  Poincare::Layout createLayout(int index);

  Stack * m_stack;
  InputController * m_inputController;
  ContentView * m_view;
  Poincare::Context * m_context;
  Escher::EvenOddExpressionCell m_cells[Stack::k_stackSize];
};

}

#endif
