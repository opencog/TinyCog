;; File: interaction-rules.scm
;; ;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; ;; License: AGPL
;; ;; Date: May, 2018

;;; -----------------------------------------------
;;; This file contains dynamic interaction rules
;;; Mainely triggered by the occurrence of events
;;; Need to be loaded after events.scm




;; ------------------------------------------------
;; Functions used by interaction rules

; The following two are used by face detection rule
(define (antecedent-face)
	(if (= (psi-get-number-value event-face) 1)
		(stv 1 1)
		(stv 0 1)
	)
)

; If face is detected set Ghost input to trigger speech output
; This is for the time being until Ghost is able to process non-speech input
(define (consequent-face)
	; test-ghost is used for the time being
	(test-ghost "inner msg face is detected")

	; Return Meaningless atom	
	(Concept "")
)



;; ----------------------------------------------------
;; Interaction rules

; Interaction rule for face detection
(psi-create-general-rule
	; Antecedent
	(EvaluationLink
		(GroundedPredicateNode "scm: antecedent-face")
		(List)
	)
	; Consequent
	(GroundedSchemaNode "scm: consequent-face")
	; Params
	(List)
)
