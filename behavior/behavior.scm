;; File: behavior.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018


;; ---------------------------------------------
;; Define functions used by the rules
(include "functions.scm")

;; Load ghost scripts
(begin (display "Loading Ghost Scripts...") (newline))
(ghost-parse-file (string-append TOPDIR "/ghost/concepts_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/intro1.ghost"))
(begin (display "Done Loading Ghost Scripts.") (newline))


;; Run ghost loop
;(ghost-run)


;; Run dynamic psi
;(psi-updater-run)

;; Behavior: where to look

; utility vars
(define no_faces)
(define smiling_face_idx)
(define loc_single_face)
(define loc_smiling_face)

; check if a face in sight
(define-public (sense-any-face?)
	(set! no_faces (car (cog-value->list (cog-value Afs Anof))))
	(if (eq? (car (cog-value->list (cog-value x y))) 0)
		(stv 0 1)
		(stv 1 1)
	)
)


(define-public (sense-any-smiling-face?)
	(if (> no_faces 0)
		(if (eq? no_faces 1)
			(set! smiling_face_idx (car (cog-value->list (cog-value (ConceptNode "face_1") Asm))))
			(set! smiling_face_idx 
)






