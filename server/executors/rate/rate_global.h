#ifndef RATE_GLOBAL_H
#define RATE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RATE_LIBRARY)
#  define RATE_EXPORT Q_DECL_EXPORT
#else
#  define RATE_EXPORT Q_DECL_IMPORT
#endif

#endif // RATE_GLOBAL_H
