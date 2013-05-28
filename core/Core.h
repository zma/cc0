#pragma once

#include <core/Misc/CompilationContext.h>

// Expressions
#include <core/CodeDom/Expression.h>
#include <core/CodeDom/BinaryExpression.h>
#include <core/CodeDom/BlockExpression.h>
#include <core/CodeDom/CallExpression.h>
#include <core/CodeDom/ExpressionTreeDumper.h>
#include <core/CodeDom/ExpressionVisitor.h>
#include <core/CodeDom/FunctionExpression.h>
#include <core/CodeDom/GotoExpression.h>
#include <core/CodeDom/LabelExpression.h>
#include <core/CodeDom/ConvertExpression.h>
#include <core/CodeDom/VariableExpression.h>
#include <core/CodeDom/IndexExpression.h>
#include <core/CodeDom/AssignExpression.h>
#include <core/CodeDom/NopExpression.h>
#include <core/CodeDom/ConditionalExpression.h>
#include <core/CodeDom/ConstantExpression.h>
#include <core/CodeDom/ProgramExpression.h>
#include <core/CodeDom/ReturnExpression.h>
#include <core/CodeDom/UnaryExpression.h>
#include <core/CodeDom/ConvertExpression.h>
#include <core/CodeDom/LoopExpression.h>
#include <core/CodeDom/CommitExpression.h>
#include <core/CodeDom/MemberExpression.h>
#include <core/CodeDom/AbortExpression.h>
#include <core/CodeDom/NewRunnerExpression.h>
#include <core/CodeDom/ArraySegmentExpression.h>
#include <core/CodeDom/InlineAssemblyExpression.h>

// Types
#include <core/Type/Type.h>
#include <core/Type/BooleanType.h>
#include <core/Type/IntegerType.h>
#include <core/Type/FloatingPointType.h>
#include <core/Type/FunctionType.h>
#include <core/Type/ArrayType.h>
#include <core/Type/LabelType.h>
#include <core/Type/PointerType.h>
#include <core/Type/StructType.h>
#include <core/Type/VoidType.h>

// Symbols
#include <core/Symbol/Symbol.h>
#include <core/Symbol/SymbolScope.h>
#include <core/Symbol/Declaration.h>
#include <core/Symbol/SymbolRef.h>

// IL
#include <core/IL/IL.h>
#include <core/IL/ILProgram.h>
#include <core/IL/ILClass.h>
#include <core/IL/ILFunction.h>

// Pass
#include <core/Pass/TypeDeduction.h>
#include <core/Pass/ConstantPropagation.h>
#include <core/Pass/SourceParser.h>
#include <core/Pass/ILGenerator.h>
#include <core/Pass/ILOptimizer.h>
#include <core/Pass/CodeGenerator.h>
#include <core/Pass/TargetOptimizer.h>


// TargetOptimizer
#include <core/Target/TargetInstruction.h>
#include <core/Target/TargetProgram.h>

// Misc and interfaces
#include <core/Misc/Location.h>
#include <core/Misc/Utils.h>
#include <core/Misc/TagContainer.h>
#include <core/Misc/ConstantValue.h>

// standard stuff
#include <stdint.h>
#include <typeinfo>
