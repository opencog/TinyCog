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

(define-public Afs (ConceptNode "face"))
(define-public Asm (PredicateNode "smile"))
(define-public Aem (PredicateNode "emotion"))
(define-public Alk (PredicateNode "look"))
(define-public Anof (PredicateNode "number_of"))
(define-public Aey (ConceptNode "eyes"))
(define-public Ahh (ConceptNode "hand"))
(define-public Afi (ConceptNode "fingers"))
(define-public Apos_h (ConceptNode "position"))
(define-public Asen_h (ConceptNode "sense"))
(define-public Arate_h (ConceptNode "rate"))



; A function which always returns false
(define-public (func-false)
	(stv 0 1)
)

; A function which always returns true
(define-public (func-true)
	(stv 1 1)
)


; remap a value from one range to another
(define-public (remap val mi1 mx1 mi2 mx2)
	(+ mi2 (/ (* (- val mi1) (- mx2 mi2)) (exact->inexact (- mx1 mi1))))
)

; invert a number between 1 and 0
(define-public (invrt val) (logxor 1 val))

; convert from image width range to head pan range
(define-public (image-width->head-pan val)
	(remap val 0 IMAGE_WIDTH 0 1)
)

; conver from image height to head tilt range
(define-public (image-height->head-tilt val)
	(remap val 0 IMAGE_HEIGHT 0 1)
)


(define-public (append-space STR) (string-append STR " "))

; Convert string to ListLink of WordNodes
(define-public (str-to-listlink txt)
	(ListLink (map WordNode (string-split txt #\ ))))

; Convert list of WordNode to string
(define-public (wordlist-to-str wrdlst)
	(string-concatenate (map append-space (map cog-name (wrdlst)))))


; Check if a number is between 0 and 1
(define-public (bn-zero-one? VAL)
	(and (>= VAL 0) (<= VAL 1))
)
