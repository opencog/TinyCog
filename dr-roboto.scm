;; File: dr_roboto.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;;         Dagim Sisay <dagiopia@gmail.com>
;; License: AGPL
;; Date: May, 2018
;;

(setlocale LC_CTYPE "en_US.UTF-8")

(define-public TOPDIR (getcwd))
(add-to-load-path TOPDIR)
(setenv "LTDL_LIBRARY_PATH" 
	(string-append (getenv "LTDL_LIBRARY_PATH") ":" (string-append TOPDIR "/build")))

(chdir "build")

(use-modules (ice-9 hash-table))

(use-modules
   (opencog)
   (opencog exec)
   (opencog query)
   (opencog nlp)
   (opencog nlp relex2logic)
   (opencog openpsi)
   (opencog ghost)
   (opencog ghost procedures))



; Load C++ detector functions
(load-extension "libdr_roboto" "init_dr_roboto")
(c-init-as (cog-atomspace))

;; ---------------------------------------------
;; Define functions used by the rules
(include-from-path "behavior/functions.scm")

; load prof eintein's commands
(include-from-path "act/cmd_einstein.scm")

; Load behavior scripts
(include-from-path "behavior/behavior.scm")

; TODO this should be changed to a practical i/o implementation.
; this is just for testing
(include-from-path "socket/socket.scm")

; load scratch interface
(include-from-path "scratch/scratch.scm")

(c-start-sensors)
