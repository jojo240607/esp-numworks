#include "rpn_stack.h"
#include "shared/poincare_helpers.h"
#include "expression/integer.h"
#include <string.h>
#include <utility>
#include <poincare/preferences.h>
#include "poincare/k_tree.h"

extern "C" {
#include "py/objstr.h"
}

using namespace Poincare;

namespace Rpn {

Stack::Element::Element() : expression("0"), approximate("0"), expressionHeight(KDFont::GlyphHeight(KDFont::Size::Large)), approximateHeight(KDFont::GlyphHeight(KDFont::Size::Large)) {
}

Stack::Element::Element(Expression &exp, Context &context) {//{
    //.symbolicComputation = SymbolicComputation::ReplaceAllSymbols}
    bool reductionFailure = false;
  Shared::PoincareHelpers::CloneAndSimplify(&exp, &context, {.target = Poincare::Internal::ReductionTarget::User}, &reductionFailure);
  exp.serialize(expression, k_expressionSize);
  if (find_subbytes((const byte*)expression, strlen(expression), (const byte*)"undef", strlen("undef"), 1) != nullptr) {
    // Expression got too large, need to approximate.
    exp = Shared::PoincareHelpers::Approximate<double>(exp, &context);
    exp.serialize(expression, k_expressionSize);
  }
  expressionHeight = exp.createLayout(
    MathPreferences::SharedPreferences()->displayMode(),
    MathPreferences::SharedPreferences()->numberOfSignificantDigits(),
    &context
  )->layoutSize(KDFont::Size::Large).height();

  Expression approx = exp.approximateToTree<double>(
    MathPreferences::SharedPreferences()->angleUnit(),
    MathPreferences::SharedPreferences()->complexFormat(),
    &context
  );
  approx.serialize(approximate, k_expressionSize);
  approximateHeight = approx.createLayout(
    MathPreferences::SharedPreferences()->displayMode(),
    MathPreferences::SharedPreferences()->numberOfSignificantDigits(),
    &context
  )->layoutSize(KDFont::Size::Large).height();
}

I18n::Message Stack::operator()(const char *text, Context *context) {
    printf("operator 4 text %s\n", text);
  Expression exp = Expression::Parse(text, context);
  if (exp.isUninitialized()) {
      printf("test 1\n");
    return I18n::Message::SyntaxError;
  }
    printf("test 2\n");
  return push(exp, *context);
}

I18n::Message Stack::operator()(StackOperation op) {
    printf("operator 3 StackOperation %d\n", op);
  Element a, b, c;
  I18n::Message r = I18n::Message::Default;

  switch (op) {
    case DUP:
      a = m_stack[0];
      r = push(a);
      break;
    case SWAP:
      a = m_stack[0];
      b = m_stack[1];
      pop();
      pop();
      push(a);
      push(b);
      break;
    case ROT:
      a = m_stack[0];
      b = m_stack[1];
      c = m_stack[2];
      pop();
      pop();
      pop();
      push(b);
      push(a);
      push(c);
      break;
    case OVER:
      a = m_stack[1];
      r = push(a);
      break;
    case POP:
      pop();
      break;
    case CLEAR:
      while (!empty()) {
        pop();
      }
      break;
  }
  return r;
}

struct PoincareOperators {
  Poincare::Internal::Type type;
  int nargs;
  Expression(*op)(const Stack&);
};

I18n::Message Stack::operator()(Poincare::Internal::Type op, Context *context) {
    printf("operator 2 op type = %d\n", op);
  const static PoincareOperators poincareOperators[] = {
    { Poincare::Internal::Type::Add, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KAdd(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { Poincare::Internal::Type::Sub, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KSub(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { Poincare::Internal::Type::Mult, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KMult(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { Poincare::Internal::Type::Div, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KDiv(KA, KB), {.KA = s(1), .KB = s(0)}); } },

    { Poincare::Internal::Type::Ln, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KLn(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::LogBase, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KLogBase(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { Poincare::Internal::Type::Pow, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KPow(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { Poincare::Internal::Type::Sin, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KSin(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::Cos, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KCos(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::Tan, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KTan(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::ASin, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KASin(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::ACos, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KACos(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::ATan, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KATan(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::Sqrt, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KSqrt(KA), {.KA = s(0)}); } },
    { Poincare::Internal::Type::Opposite, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KOpposite(KA), {.KA = s(0)}); } },

    { Poincare::Internal::Type::Store, 2, [](const Stack& s) -> Expression {
      auto symbol = s(0);

      if (symbol.isOfType({Poincare::Internal::Type::UserSymbol})) {
          return UserExpression::Create(KStore(KA, KB), {.KA = s(1), .KB = symbol});
        //return Store::Builder(s(1), *reinterpret_cast<UserSymbol*>(&symbol));
      }
      return Undefined::Builder();
    } },
  } ;

  for (size_t i = 0; i < sizeof(poincareOperators)/sizeof(poincareOperators[0]); i++) {
    if (poincareOperators[i].type == op) {
        printf("doOperation\n");
      return doOperation(std::move(poincareOperators[i].op(*this)), *context, poincareOperators[i].nargs);
    }
  }
  return I18n::Message::Warning;
}

struct SpecialOperators {
  Stack::SpecialOperation type;
  int nargs;
  Expression(*op)(const Stack&);
};

I18n::Message Stack::operator()(Stack::SpecialOperation op, Context *context) {
    printf("operator 1\n");
  const static SpecialOperators specialOperators[] = {
    { Stack::Exp, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KPow(KA, KB), {.KB = s(0)}); } },
    { Stack::CommonLogarithm, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KLog(KA), {.KA = s(0)}); } },
    { Stack::Square, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KPow(KA, KB), {.KA = s(0)}); } },
  } ;

  for (size_t i = 0; i < sizeof(specialOperators)/sizeof(specialOperators[0]); i++) {
    if (specialOperators[i].type == op) {
      return doOperation(std::move(specialOperators[i].op(*this)), *context, specialOperators[i].nargs);
    }
  }
  return I18n::Message::Warning;
}

struct MessageOperators {
  I18n::Message type;
  int nargs;
  Expression(*op)(const Stack&);
};

I18n::Message Stack::operator()(I18n::Message op, Context *context) {
    printf("operator 0\n");
  const static MessageOperators messageOperators[] = {
    { I18n::Message::AbsCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KAbs(KA), {.KA = s(0)}); } },
    { I18n::Message::AcoshCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KACos(KA), {.KA = s(0)}); } },
    { I18n::Message::ArgCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KArg(KA), {.KA = s(0)}); } },
    { I18n::Message::AsinhCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KASin(KA), {.KA = s(0)}); } },
    { I18n::Message::AtanhCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KTan(KA), {.KA = s(0)}); } },
   // { I18n::Message::BinomialCommandWithArg, 2, [](const Stack& s) -> Expression { return BinomialCoefficient::Builder(s(1), s(0)); } },
    { I18n::Message::BinomialCDFCommandWithArg, 3, [](const Stack& s) -> Expression { return UserExpression::Create(KBinomial(KA, KB), {.KA = s(2), .KB = s(1), .KC = s(0)}); } },
    { I18n::Message::BinomialPDFCommandWithArg, 3, [](const Stack& s) -> Expression { return UserExpression::Create(KBinomialL(KA, KB), {.KA = s(2), .KB = s(1), .KC = s(0)});; } },
    { I18n::Message::CeilCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KCeil(KA), {.KA = s(0)}); } },
   // { I18n::Message::ConfidenceCommandWithArg, 2, [](const Stack& s) -> Expression { return ConfidenceInterval::Builder(s(1), s(0)); } },
  //  { I18n::Message::ConjCommandWithArg, 1, [](const Stack& s) -> Expression { return Conjugate::Builder(s(0)); } },
    { I18n::Message::CoshCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KCosH(KA), {.KA = s(0)}); } },
    { I18n::Message::DeterminantCommandWithArg, 1, [](const Stack& s) -> Expression { UserExpression::Create(KDet(KA), {.KA = s(0)}); } },

    //{ I18n::Message::DiffCommandWithArg, 3, [](const Stack& s) -> Expression {
    //  auto symbol = s(1);
    //  if (symbol.type() == Poincare::Internal::Type::Symbol) {
    //    return Derivative::Builder(s(2), *reinterpret_cast<Symbol*>(&symbol), s(0));
    //  }
    //  return Undefined::Builder();
    //} },

    { I18n::Message::DimensionCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KMatrix1x1L(KA), {.KA = s(0)}); } },
    { I18n::Message::FactorCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KFactor(KA), {.KA = s(0)}); } },
    { I18n::Message::FloorCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KFloor(KA), {.KA = s(0)}); } },
    { I18n::Message::FracCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KFrac(KA), {.KA = s(0)}); } },
    { I18n::Message::GcdCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KDiv(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { I18n::Message::ImCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KIm(KA), {.KA = s(0)});  } },
    { I18n::Message::IndentityCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KMatrix1x1L(KA), {.KA = s(0)}); } },

    //{ I18n::Message::IntCommandWithArg, 4, [](const Stack& s) -> Expression {
    //  auto symbol = s(2);
    //  if (symbol.type() == Poincare::Internal::Type::Symbol) {
    //    return Integral::Builder(s(3), *reinterpret_cast<Symbol*>(&symbol), s(1), s(0));
    //  }
    //  return Undefined::Builder();
    //} },

    { I18n::Message::InvBinomialCommandWithArg, 3, [](const Stack& s) -> Expression { return UserExpression::Create(KBinomial(KA, KB), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::InverseCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KMatrix1x1L(KA), {.KA = s(0)}); } },
    { I18n::Message::InvNormCommandWithArg, 3, [](const Stack& s) -> Expression { return UserExpression::Create(KInverse(KA), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::LcmCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KLCM(KA, KB), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::LogCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KLogBase(KA, KB), {.KA = s(1), .KB = s(0)}); } },
    { I18n::Message::NormCDFCommandWithArg, 3, [](const Stack& s) -> Expression { return UserExpression::Create(KNorm(KA), {.KA = s(0), .KB = s(1)}); } },
    //{ I18n::Message::NormCDF2CommandWithArg, 4, [](const Stack& s) -> Expression { return NormCDF2::Builder(s(3), s(2), s(1), s(0)); } },
    { I18n::Message::NormPDFCommandWithArg, 3, [](const Stack& s) -> Expression { return UserExpression::Create(KNorm(KA), {.KA = s(0), .KB = s(1)}); } },
    //{ I18n::Message::Prediction95CommandWithArg, 2, [](const Stack& s) -> Expression { return SimplePredictionInterval::Builder(s(1), s(0)); } },
    //{ I18n::Message::PredictionCommandWithArg, 2, [](const Stack& s) -> Expression { return PredictionInterval::Builder(s(1), s(0)); } },

    //{ I18n::Message::ProductCommandWithArg, 4, [](const Stack& s) -> Expression {
    //  auto symbol = s(2);
    //  if (symbol.type() == Poincare::Internal::Type::Symbol) {
    //    return Product::Builder(s(3), *reinterpret_cast<Symbol*>(&symbol), s(1), s(0));
    //  }
    //  return Undefined::Builder();
    //} },

    { I18n::Message::QuoCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KDiv(KA, KB), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::RandintCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KRandInt(KA, KB), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::RandomCommandWithArg, 0, [](const Stack& s) -> Expression { return UserExpression::Create(KRandom, {}); } },
    { I18n::Message::ReCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KReal(KA), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::RemCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KDiv(KA, KB), {.KA = s(0), .KB = s(1)});UserExpression::Create(KDiv(KA, KB), {.KA = s(0), .KB = s(1)}); } },
    //{ I18n::Message::RootCommandWithArg, 2, [](const Stack& s) -> Expression { return NthRoot::Builder(s(1), s(0)); } },
    { I18n::Message::RoundCommandWithArg, 2, [](const Stack& s) -> Expression { return UserExpression::Create(KRound(KA, KB), {.KA = s(0), .KB = s(1)}); } },
    { I18n::Message::SinhCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KSinH(KA), {.KA = s(0), }); } },

    //{ I18n::Message::SumCommandWithArg, 4, [](const Stack& s) -> Expression {
    //  auto symbol = s(2);
    //  if (symbol.type() == Poincare::Internal::Type::Symbol) {
    //    return Sum::Builder(s(3), *reinterpret_cast<Symbol*>(&symbol), s(1), s(0));
    //  }
    //  return Undefined::Builder();
    //} },

    { I18n::Message::TanhCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KTan(KA), {.KA = s(0), }); } },
    { I18n::Message::TraceCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KTrace(KA), {.KA = s(0), }); } },
    { I18n::Message::TransposeCommandWithArg, 1, [](const Stack& s) -> Expression { return UserExpression::Create(KTranspose(KA), {.KA = s(0), }); } },
  } ;

  for (size_t i = 0; i < sizeof(messageOperators)/sizeof(messageOperators[0]); i++) {
    if (messageOperators[i].type == op) {
      return doOperation(std::move(messageOperators[i].op(*this)), *context, messageOperators[i].nargs);
    }
  }

  return I18n::Message::Warning;
}

void Stack::dropNth(size_t index) {
  if (!empty()) {
    for (size_t i = index; i < length(); i++) {
      m_stack[i] = m_stack[i+1];
    }
    m_length -= m_length > 0 ? 1 : 0;
    m_stack[k_stackSize-1] = Element();
  }
}

I18n::Message Stack::doOperation(Expression e, Context &context, int nargs) {
  if (e.isUndefined()) {
    return I18n::Message::SyntaxError;
  }
  else if (e.hasRandomNumber()) {
    // We don't want numbers on the stack to change on each evaluation.
    e = Shared::PoincareHelpers::Approximate<double>(e, &context);
  }
  if ((length() + nargs + 1) > (k_stackSize + nargs + 1)) {
    return I18n::Message::StorageMemoryFull;
  }
  while(nargs) {
    pop();
    nargs--;
  }
  return push(std::move(e), context);
}

I18n::Message Stack::push(Expression e, Context &context) {
  Element a(e, context);
  return push(std::move(a));
}

I18n::Message Stack::push(Element e) {
  if (full()) {
    return I18n::Message::StorageMemoryFull;
  }
  for (size_t i = k_stackSize-1; i > 0; i--) {
    m_stack[i] = m_stack[i-1];
  }
  m_stack[0] = e;
  m_length++;
    printf("m_length = %d\n", m_length);
  return I18n::Message::Default;
}

void Stack::pop() {
  for (size_t i = 0; i < k_stackSize-1; i++) {
    m_stack[i] = m_stack[i+1];
  }
  m_stack[k_stackSize-1] = Element();
  m_length -= m_length > 0 ? 1 : 0;
}

}
