#ifndef RPN_CONTENT_VIEW_H
#define RPN_CONTENT_VIEW_H


#include "escher/view.h"
#include "escher/responder.h"
#include "escher/selectable_table_view.h"
#include "escher/text_field.h"

namespace Rpn {
  class StackController;
  class InputController;

class ContentView : public Escher::View {
public:
  ContentView(Escher::Responder * parentResponder, InputController * inputController, StackController * stackController);
  ~ContentView();
  ContentView(const ContentView& other) = delete;
  ContentView(ContentView&& other) = delete;
  ContentView& operator=(const ContentView& other) = delete;
  ContentView& operator=(ContentView&& other) = delete;
  void reload();
  Escher::SelectableTableView * stackView() { return &m_stackView; }
  Escher::TextField * inputView() { return &m_inputView; }
  /* View */
  int numberOfSubviews() const override { return 2; }
  Escher::View * subviewAtIndex(int index) override;

private:
    constexpr static KDGlyph::Format k_inputFormat = {
            .style = {.glyphColor = KDColorBlack,
                    .backgroundColor = Escher::Palette::GrayMiddle,
                    .font = KDFont::Size::Large},
            .horizontalAlignment = KDGlyph::k_alignRight,
            .verticalAlignment = KDGlyph::k_alignCenter};
  void layoutSubviews(bool force = false) override;

  Escher::SelectableTableView m_stackView;
  Escher::TextField m_inputView;
  char m_textBuffer[Escher::TextField::MaxBufferSize()];
};

}

#endif
