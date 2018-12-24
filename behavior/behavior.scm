;; File: behavior.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;;         Dagim Sisay <dagiopia@gmail.com>
;; License: AGPL
;; Date: May, 2018


;; ---------------------------------------------
;; Define functions used by the rules
(include-from-path "behavior/functions.scm")

;; Load ghost scripts
(begin (display "Loading Ghost Scripts...") (newline))
(ghost-parse-file (string-append TOPDIR "/ghost/concepts_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/intro1.ghost"))
(begin (display "Done Loading Ghost Scripts.") (newline))


;; Behavior: where to look

; utility vars
(define no_faces)
(define smiling_face_idx)
(define nonneutral-face-idx)
(define loc_smiling_face)

; check if a face in sight
(define-public (sense-any-face?)
	(set! no_faces (car (cog-value->list (cog-value Afs Anof))))
	(if (eq? no_faces 0)
		(cog-new-stv 0 1)
		(cog-new-stv 1 1)))

; generic check if any function
; an evaluationlink sends a node to get how many of it 
; is sensed. The sensors would set a floatvalue with 
; the arg node as the atom and the (PredicateNode "number_of") as key
(define-public (sense-any? atom)
	(set! no_faces (car (cog-value->list (cog-value atom Anof))))
	(if (eq? no_faces 0)
		(cog-new-stv 0 1)
		(cog-new-stv 1 1)))


(define-public (sense-find-smiling-face)
	(set! smiling_face_idx 0)
	(if (> no_faces 0)
		(if (eq? no_faces 1) ; if there is only one face, this doesn't matter!
			(set! smiling_face_idx (car (cog-value->list (cog-value (ConceptNode "face_1") Asm))))
			(do ((i 2 (1+ i))) ((> i no_faces)) 
				(if (> (car (cog-value->list (cog-value (ConceptNode (format #f "face_~d" i)) Asm))) 0)
					(set! smiling_face_idx i))))))

; set the index of a non neutral face to nonneutral-face-idx
(define-public (sense-find-emotional-face)
	(set! nonneutral-face-idx 0)
	(if (> no_faces 0)
		(do ((i 2 (1+ i))) ((> i no_faces))
			(if (not (eq? "neutral"
			              (car (cog-value->list (cog-value Aem (ConceptNode (format #f "face_~d" i)))))))
				(set! nonneutral-face-idx i)))))


; before looking at face check if any faces first and return false if none
(define-public (look-face atom)
	(SequentialAndLink 
		(EvaluationLink 
			(GroundedPredicateNode "scm: sense-any?")
			atom)
		(cog-evaluate! behave-looking-face))) ; go to selecting which face to see after
		                                      ; making sure there is indeed a face

; look at a single face
; first check if only one face in view
(define (look-single-face atom)
	(if (eq? no_faces 1)
		(begin 
			(act-face-a-point (cog-value->list (cog-value (ConceptNode "face_1") Apos_h)))
			(cog-new-stv 1 1))
		(cog-new-stv 0 1))) ; fail -> more than one face


; face a smiling face if any
(define (look-smiling-face atom)
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
(define-public (look-normal-face atom)
	(if (eq? no_faces 0)
		(cog-new-stv 0 1)
		(begin
			(act-face-a-point (cog-value->list
			                     (cog-value
			                        (ConceptNode (format #f "face_~d" (random no_faces)))
			                        Apos_h)))
			(cog-new-stv 1 1))))


; look at a salient point
(define-public (look-salient-point atom)
	(begin
		(act-face-a-point (cog-value->list (cog-value Aey Apos_h)))
		(cog-new-stv 1 1)))


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
			(GroundedPredicateNode "scm: look-normal-face")
			(PredicateNode "look"))))


(define-public behave-looking
	(SequentialOrLink
		(EvaluationLink
			(GroundedPredicateNode "scm: look-face")
			(ConceptNode "face"))
		(EvaluationLink
			(GroundedPredicateNode "scm: look-salient-point")
			(PredicateNode "look"))))



