;; File: dr_roboto.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;;         Dagim Sisay <dagiopia@gmail.com>
;; License: AGPL
;; Date: May, 2018
;;


(setlocale LC_CTYPE "C")

(define-public TOPDIR (current-filename))
(set! TOPDIR (string-drop-right TOPDIR
	(-
		(- (string-length TOPDIR) 1)
		(string-rindex TOPDIR #\/)
	))
)


(add-to-load-path "/usr/local/share/opencog/scm")

; Load C++ detector functions
(load-extension "libdr_roboto" "init_dr_roboto")


; Load behavior scripts
(include "behavior/behavior.scm")

; TODO this should be changed to a practical i/o implementation.
; this is just for testing
(include "socket/socket.scm")

; load scratch interface
(include "scratch/scratch.scm")
