#ifndef CORE_SERIALIZATION_EXPORT_DERIVE_EXPRESSIONS_H
#define CORE_SERIALIZATION_EXPORT_DERIVE_EXPRESSIONS_H

#include "core/CodeDom/Expression.h"
#include "core/CodeDom/AbortExpression.h"
#include "core/CodeDom/ArraySegmentExpression.h"
#include "core/CodeDom/AssignExpression.h"
#include "core/CodeDom/BinaryExpression.h"
#include "core/CodeDom/BlockExpression.h"
#include "core/CodeDom/CallExpression.h"
#include "core/CodeDom/CommitExpression.h"
#include "core/CodeDom/ConditionalExpression.h"
#include "core/CodeDom/ConstantExpression.h"
#include "core/CodeDom/ConvertExpression.h"
#include "core/CodeDom/FunctionExpression.h"
#include "core/CodeDom/GotoExpression.h"
#include "core/CodeDom/IndexExpression.h"
#include "core/CodeDom/InlineAssemblyExpression.h"
#include "core/CodeDom/LabelExpression.h"
#include "core/CodeDom/LoopExpression.h"
#include "core/CodeDom/MemberExpression.h"
#include "core/CodeDom/NewRunnerExpression.h"
#include "core/CodeDom/NopExpression.h"
#include "core/CodeDom/ProgramExpression.h"
#include "core/CodeDom/ReturnExpression.h"
#include "core/CodeDom/UnaryExpression.h"
#include "core/CodeDom/VariableExpression.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(AbortExpression);
BOOST_CLASS_EXPORT(ArraySegmentExpression);
BOOST_CLASS_EXPORT(AssignExpression);
BOOST_CLASS_EXPORT(BinaryExpression);
BOOST_CLASS_EXPORT(BlockExpression);
BOOST_CLASS_EXPORT(CallExpression);
BOOST_CLASS_EXPORT(CommitExpression);
BOOST_CLASS_EXPORT(ConditionalExpression);
BOOST_CLASS_EXPORT(ConstantExpression);
BOOST_CLASS_EXPORT(ConvertExpression);
BOOST_CLASS_EXPORT(FunctionExpression);
BOOST_CLASS_EXPORT(GotoExpression);
BOOST_CLASS_EXPORT(IndexExpression);
BOOST_CLASS_EXPORT(InlineAssemblyExpression);
BOOST_CLASS_EXPORT(LabelExpression);
BOOST_CLASS_EXPORT(LoopExpression);
BOOST_CLASS_EXPORT(MemberExpression);
BOOST_CLASS_EXPORT(NewRunnerExpression);
BOOST_CLASS_EXPORT(NopExpression);
BOOST_CLASS_EXPORT(ProgramExpression);
BOOST_CLASS_EXPORT(ReturnExpression);
BOOST_CLASS_EXPORT(UnaryExpression);
BOOST_CLASS_EXPORT(VariableExpression);

#endif
