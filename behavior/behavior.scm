;; File: behavior.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;;         Dagim Sisay <dagiopia@gmail.com>
;; License: AGPL
;; Date: May, 2018


;; load ghost rules
(begin (display "Loading Ghost Scripts...") (newline))
;(ghost-parse-file (string-append TOPDIR "/ghost/concepts_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/intro1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/chitchat.ghost"))
(begin (display "Done Loading Ghost Scripts.") (newline))


;; Behavior: where to look
; utility vars
(define-public no_faces 0)
(define-public smiling_face_idx 0)
(define-public nonneutral-face-idx 0)
(define-public loc_smiling_face 0)

; check if a face in sight
(define-public (sense-any-face? atom)
	(begin (display "** sensing for a face **") (newline))
	(set! no_faces (cog-value->exact (cog-value Afs Anof)))
	(if (> no_faces 0)
		(cog-new-stv 1 1)
		(cog-new-stv 0 1)))

; generic check if any function
; an evaluationlink sends a node to get how many of it 
; is sensed. The sensors would set a floatvalue with 
; the arg node as the atom and the (PredicateNode "number_of") as key
(define-public (sense-any? atom)
	(begin (display "** generic sense any? **") (newline))
	(set! no_faces (cog-value->exact (cog-value atom Anof)))
	(if (> no_faces 0)
		(cog-new-stv 1 1)
		(cog-new-stv 0 1)))


(define-public (sense-find-smiling-face)
	(begin (display "** sensing for smiling face **") (newline))
	(set! smiling_face_idx 0)
	(if (> no_faces 0)
		(do ((i 1 (+ 1 i))) ((> i no_faces))
			(if (> (car (cog-value->list (cog-value (ConceptNode (format #f "face_~d" i)) Asm))) 0)
				(set! smiling_face_idx i)))))

; set the index of a non neutral face to nonneutral-face-idx
(define-public (sense-find-emotional-face)
	(begin (display "** sensing for an emotional face **") (newline))
	(set! nonneutral-face-idx 0)
	(if (> no_faces 0)
		(do ((i 2 (1+ i))) ((> i no_faces))
			(if (not (string=? "neutral"
			              (car (cog-value->list (cog-value Aem (ConceptNode (format #f "face_~d" i)))))))
				(set! nonneutral-face-idx i)))))

; before looking at face check if any faces first and return false if none
(define-public (look-face atom)
	(begin (display "** executing behave find face to look at **") (newline))
	(cog-evaluate! behave-find-face-to-look))


(define-public (look-select-face atom)
	(cog-evaluate! behave-looking-face))


; look at a single face
; first check if only one face in view
(define-public (look-single-face atom)
	(begin (display "**> look single face **") (newline))
	(if (eq? no_faces 1)
		(begin 
			(act-face-a-point (cog-value->list (cog-value (ConceptNode "face_1") Apos_h)))
			(cog-new-stv 1 1))
		(cog-new-stv 0 1))) ; fail -> more than one face


; face a smiling face if any
(define-public (look-smiling-face atom)
	(begin (display "**> look smiling face **") (newline))
	(sense-find-smiling-face)
	(if (eq? smiling_face_idx 0) ; if smiling_face_idx is zero, there is no smiling face
		(cog-new-stv 0 1)         ; so return failure
		(begin
			(act-face-a-point (cog-value->list
			                    (cog-value
			                      (ConceptNode (format #f "face_~d" smiling_face_idx))
			                      Apos_h)))
			(cog-new-stv 1 1))))


; face an emotional face, a face which is one of angry, sad, surprised or happy
; in general any face which isn't neutral
; set something here to aid a ghost gambit in saying "why the _____ face"
(define-public (look-emotional-face atom)
	(begin (display "**> look emotional face **") (newline))
	(sense-find-emotional-face)
	(if (eq? nonneutral-face-idx 0)
		(cog-new-stv 0 1)
		(begin
			(act-face-a-point (cog-value->list
			                     (cog-value
			                        (ConceptNode (format #f "face_~d" nonneutral-face-idx))
			                        Apos_h)))
			(cog-new-stv 1 1))))

; face one of the neutral faces just at random
(define-public (look-random-face atom)
	(begin (display "**> look random face **") (newline))
	(if (eq? no_faces 0)
		(cog-new-stv 0 1)
		(begin
			(act-face-a-point (cog-value->list
			                     (cog-value
			                        (ConceptNode (format #f "face_~d" (random no_faces)))
			                        Apos_h)))
			(cog-new-stv 1 1))))

; set some deault values
(cog-set-value! Aey Apos_h (FloatValue 0 0))

; look at a salient point
(define-public (look-salient-point atom)
	(begin (display "**> look at salient point **") (newline))
	(begin
		(act-face-a-point (cog-value->list (cog-value Aey Apos_h)))
		(cog-new-stv 1 1)))


(define-public behave-find-face-to-look
	(SequentialAndLink 
		(EvaluationLink 
			(GroundedPredicateNode "scm: sense-any-face?") 
			(ConceptNode "face"))
		(EvaluationLink
			(GroundedPredicateNode "scm: look-select-face")
			(ConceptNode "face"))))
			

; selector bt for looking at an interesting or a single face
(define-public behave-looking-face
	(SequentialOrLink
		(EvaluationLink
			(GroundedPredicateNode "scm: look-single-face")
			(PredicateNode "look"))
		(EvaluationLink
			(GroundedPredicateNode "scm: look-smiling-face")
			(PredicateNode "look"))
		(EvaluationLink
			(GroundedPredicateNode "scm: look-emotional-face")
			(PredicateNode "look"))
		(EvaluationLink
			(GroundedPredicateNode "scm: look-random-face")
			(PredicateNode "look"))))

; looking behavior... first to try to look at a face, an interesting one
; if possible then if no face at all, just move on to looking at a salient
; point
(define-public behave-looking
	(SequentialOrLink
		(EvaluationLink
			(GroundedPredicateNode "scm: look-face")
			(ConceptNode "face"))
		(EvaluationLink
			(GroundedPredicateNode "scm: look-salient-point")
			(PredicateNode "look"))))

(define-public (oc-bt)
	(while #t 
		(begin (display "** executing main BT **") (newline))
	       (cog-evaluate! behave-looking)
	       (usleep 5000000)))


(call-with-new-thread oc-bt)
