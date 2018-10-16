#!/usr/local/bin/guile -l
!#

; File: TestDrRoboto.scm
; Author: Dagim Sisay <dagiopia@gmail.com>
; License: AGPL
; Date: October, 2018

(use-modules (ice-9 readline)) (activate-readline)
(add-to-load-path "/usr/local/share/opencog/scm")
(add-to-load-path ".")
(use-modules (opencog))
(use-modules (opencog query))
(use-modules (opencog exec))
(load-from-path "opencog.scm")

(define-public (count-atoms)
	(define cnt 0)
	(define (incrm a) (set! cnt (+ cnt 1)) #f)
	(define (cnt-type x) (cog-map-type incrm x)) 
	(map cnt-type (cog-get-types))
	cnt
)


(load-extension "./libdr_roboto" "init_dr_roboto")
(c-init-as (cog-atomspace))

(cond 
	((= (length (command-line)) 2)  
	    (display "use ice-9 getopt-long for taking args" ;(cadr (command-line))
	    )
	)
)

(c-start-sensors)
