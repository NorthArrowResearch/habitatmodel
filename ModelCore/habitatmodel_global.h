#ifndef HABITATMODEL_GLOBAL_H
#define HABITATMODEL_GLOBAL_H

#if defined(_WIN32) || defined(_WIN64)
#   ifdef MY_DLL_EXPORT
#       define DLL_API __declspec(dllexport)
#   else
#       define DLL_API __declspec(dllimport)
#   endif
#else
#   define DLL_API
#endif

#endif // HABITATMODEL_GLOBAL_H
