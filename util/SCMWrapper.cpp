#include "util/SCMWrapper.hpp"

SCMWrapper::SCMWrapper(const char *call_name, const char * (*f) ())
{
  func_ptr = f;                                             
  scm_c_define_gsubr("call_name", 0,0,0, C(func_wrapper));
  scm_c_export(call_name, NULL);                           
}


SCM SCMWrapper::func_wrapper(const char * (*f) ())
{
  return scm_from_locale_string(f());
}
