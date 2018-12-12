;; File: dr_roboto.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;;         Dagim Sisay <dagiopia@gmail.com>
;; License: AGPL
;; Date: May, 2018
;;

(setlocale LC_CTYPE "C")
(define TOPDIR (getcwd))

(add-to-load-path "/usr/local/share/opencog")
(add-to-load-path TOPDIR)

(use-modules (ice-9 hash-table))

(use-modules
   (opencog)
   (opencog exec)
   (opencog query)
   (opencog nlo)
   (opencog nlp relex2logic)
   (opencog ghost)
   (opencog ghost procedures))



; Load C++ detector functions
(load-extension "libdr_roboto" "init_dr_roboto")
(c-init-as (cog-atomspace))


; Load behavior scripts
(include "behavior/behavior.scm")

; TODO this should be changed to a practical i/o implementation.
; this is just for testing
(include "socket/socket.scm")

; load scratch interface
(include "scratch/scratch.scm")

; load prof eintein's commands
(include "act/cmd_einstein.scm")

(c-start-sensors)
