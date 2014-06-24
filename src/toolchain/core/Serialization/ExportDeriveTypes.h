#ifndef CORE_SERIALIZATION_EXPORT_DERIVE_TYPES_H
#define CORE_SERIALIZATION_EXPORT_DERIVE_TYPES_H

#include "core/Symbol/Declaration.h"
#include "core/Type/Type.h"
#include "core/Type/ArrayType.h"
#include "core/Type/BooleanType.h"
#include "core/Type/FloatingPointType.h"
#include "core/Type/FunctionType.h"
#include "core/Type/IntegerType.h"
#include "core/Type/LabelType.h"
#include "core/Type/PointerType.h"
#include "core/Type/StructType.h"
#include "core/Type/VoidType.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(ArrayType);
BOOST_CLASS_EXPORT(BooleanType);
BOOST_CLASS_EXPORT(FloatingPointType);
BOOST_CLASS_EXPORT(FunctionType);
BOOST_CLASS_EXPORT(IntegerType);
BOOST_CLASS_EXPORT(LabelType);
BOOST_CLASS_EXPORT(PointerType);
BOOST_CLASS_EXPORT(StructType);
BOOST_CLASS_EXPORT(VoidType);

#endif

