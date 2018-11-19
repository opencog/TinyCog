; guile module for emotion recognition demo

(setlocale LC_CTYPE "C")
(define TOPDIR (getcwd))

(add-to-load-path "/usr/local/share/opencog")
(add-to-load-path TOPDIR)

(load-extension "build/libemo_demo" "init_emo_demo")

; utility
(define t (stv 1 1))
(define f (stv 0 1))

; functions for ghost
(define-public (func-face-seen) (if (not (equal? (car (string-split (det-emo) #\,)) "")) t f))
(define-public (func-no-face-seen) (if (equal? (car (string-split (det-emo) #\,))  "") t f))
(define-public (func-happy-face-seen) (if (equal? (car (string-split (det-emo) #\,)) "happy") t f))
(define-public (func-sad-face-seen) (if (equal? (car (string-split (det-emo) #\,))  "sad") t f))
(define-public (func-angry-face-seen) (if (equal? (car (string-split (det-emo) #\,))  "angry") t f))
(define-public (func-neutral-face-seen) (if (equal? (car (string-split (det-emo) #\,))  "neutral") t f))


(use-modules (opencog)
             (opencog nlp)
             (opencog nlp relex2logic)
             (opencog openpsi)
             (opencog ghost)
             (opencog ghost procedures))

(begin (display "Loading Ghost Scripts...") (newline))
;(ghost-parse-file (string-append TOPDIR "/ghost/concepts.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character1.ghost"))
(begin (display "Done Loading Ghost Scriptas") (newline))

(include "socket/socket.scm")

(define txt-str-prev)

; loop on ghost
#!
(while #t
	(let ((txt-str "") (ghost-result '()))
		(set! ghost-result (ghost-get-result))
		(set! txt-str (string-join (map cog-name (ghost-get-result)) " "))
		(if (equal? txt-str txt-str-prev)
			(continue)
			(begin 
				(act-say txt-str)
				(begin (display txt-str) (newline))
				(set! txt-str-prev txt-str)
			)
		)
		(usleep 10000)
	)
)
!#
