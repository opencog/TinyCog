;; File: functions.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018

;;;; These file contains functions used by Ghost or OpenPsi rules
;;;; Rule is like	context ==> action

;; Functions for context part
;; Need to return either (stv 0 1) or (stv 1 1) which are like false and true


; Atoms of sensors
;; Sensor values are stored in the atomspace using values with 
;; concept and predicate nodes as key pairs.
;; For example, when a face is detected, a new node is created.
;; If it is the first face, it is given the name "face_0"
;; When the emotion detector detects that this face is angry
;; following is created:
;;      Value
;;         PredicateNode "emotion"
;;         ConceptNode "face_0"
;;         StringValue "angry"
;;
;; The same thing for the other sensors. 
;; 
;; The "sense" and "rate" nodes are used to set the delay for
;; each loop of the sensors. Setting the following delays each
;; sensor iteration by 1000 us
;;      (cog-set-value sen_h rate_h (FloatValue 1000))

(define-public sm (PredicateNode "smile"))
(define-public lk (PredicateNode "look"))
(define-public ey (ConceptNode "eyes"))
(define-public hh (ConceptNode "hand"))
(define-public fi (ConceptNode "fingers"))
(define-public pos_h (ConceptNode "position"))
(define-public sen_h (ConceptNode "sense"))
(define-public rate_h (ConceptNode "rate"))



; A function which always returns false
(define-public (func-false)
	(stv 0 1)
)

; A function which always returns true
(define-public (func-true)
	(stv 1 1)
)

; Convert string to ListLink of WordNodes
(define-public (str-to-listlink txt)
	(ListLink (map WordNode (string-split txt #\ ))))


