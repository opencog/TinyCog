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


