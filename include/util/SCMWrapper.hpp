/*
  File: SCMWrapper.hpp
  Author: Eskender Besrat <eskenderbesrat@gmail.com>
          Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
  Date: May, 2018
*/

#ifndef _SCMWRAPPER_HPP_
#define _SCMWRAPPER_HPP_

#include <libguile.h>


#ifdef HAVE_GUILE2
 #define C(X) ((scm_t_subr) X)
#else
 #define C(X) ((SCM (*) ()) X)
#endif


class SCMWrapper {
    
    public:
      SCMWrapper(const char *call_name, const char * (*f)());
      ~SCMWrapper() {}

    private:
      const char * (*func_ptr)();
      static SCM func_wrapper();
};

extern "C" {
    void init_dr_roboto();
};

#endif //_SCMWRAPPER_HPP_
