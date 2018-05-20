;; File: functions.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018

;;;; These file contains functions used by Ghost or OpenPsi rules
;;;; Rule is like	context ==> action

;; Functions for context part
;; Need to return either (stv 0 1) or (stv 1 1) which are like false and true

; A function which always returns false
(define-public (func-false)
	(stv 0 1)
)

; A function which always returns true
(define-public (func-true)
	(stv 1 1)
)


; Functions with arguments
(define-public (is-happy argWord)
	(define thresh (string->number (cog-name argWord)))
	(if (>= (/ (modulo (psi-loop-count (ghost-get-component)) 10) 10.0) thresh)
		(stv 1 1)
		(stv 0 1)
	)
)

; This function returns the negation of is-happy since ghost doesn't support negation on functions yet
(define-public (neg-is-happy argWord)
	(define thresh (string->number (cog-name argWord)))
	(if (>= (/ (modulo (psi-loop-count (ghost-get-component)) 10) 10.0) thresh)
		(stv 0 1)
		(stv 1 1)
	)
)

; Functions for action part
;; Need to return word nodes in a listlink
;; They need also be grounded functions


;Function to return the killers 
(define-public (func-findkiller)
  ; ... the process of finding the killer ...
  ; The answer should be a list of nodes wrapped in a ListLink
  (List (Word "Bob") (Word "and") (Word "Alice")))

; Sample returning detector function
(define-public (func-returning-result)
	
	(List (Word (number->string(detector-returning-func))))
)

; Sample looping detector function
(define-public (func-looping-result)
	; Check first if result did not expire
	(if (< (- (current-time) (hash-ref detector-looping-func-hashT "time"))  5)
		(List (Word (number->string
				(hash-ref detector-looping-func-hashT "val"))))
		(List (Word " "))
	)
)

