;; File: dr_roboto.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;;         Dagim Sisay <dagiopia@gmail.com>
;; License: AGPL
;; Date: May, 2018
;;

(add-to-load-path "/usr/local/share/opencog/scm")

(use-modules
   (opencog)
   (opencog exec)
   (opencog atom-types)
   (opencog pointmem)
   (opencog nlp relex2logic)
   (opencog openpsi)
   (opencog openpsi dynamics)
   (opencog eva-behavior)
   (opencog nlp )
   (opencog ghost))


(setlocale LC_CTYPE "C")

(define-public TOPDIR (current-filename))
(set! TOPDIR (string-drop-right TOPDIR
	(-
		(- (string-length TOPDIR) 1)
		(string-rindex TOPDIR #\/)
	))
)

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


(define txt-str-prev "")

(while #t
	(let ((txt-str "")
	     (ghost-result '()))
		

		; Finally send text output to stt when there is one
		; Note: This code is only for demo (stt input should be read from port)
      
		(set! ghost-result (ghost-get-result))
		(for-each (lambda (a)
		    (set! txt-str (string-trim (string-append txt-str " " (cog-name a))))
		    )ghost-result
		)

		(if (equal? txt-str txt-str-prev)
			(continue)
			(begin
				(act-say txt-str)
				(set! txt-str-prev txt-str)
			)

		)
	
	)
	(usleep 100000)
	
)
