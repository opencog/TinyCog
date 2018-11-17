; guile module for emotion recognition demo

(setlocale LC_CTYPE "C")
(define-public TOPDIR (dirname (getcwd)))

(add-to-load-path "/usr/local/share/opencog")
(add-to-load-path TOPDIR)

(load-extension "build/libemo_demo" "init_emo_demo")

(define-public (func-face-seen) (if (not (equal? (det-emo) "")) #t #f))
(define-public (func-face-unseen) (if (equal? (det-emo) "") #t #f))
(define-public (func-face-happy) (if (equal? (det-emo) "happy") #t #f))
(define-public (func-face-angry) (if (equal? (det-emo) "angry") #t #f))
(define-public (func-face-sad) (if (equal? (det-emo) "sad") #t #f))
(define-public (func-face-neutral) (if (equal? (det-emo) "neutral") #t #f))


(use-modules (opencog)
             (opencog nlp)
             (opencog nlp relex2logic)
             (opencog openpsi)
             (opencog ghost)
             (opencog ghost procedures))

(ghost-parse-file (string-append TOPDIR "/ghost/concepts.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character1.ghost"))

(include "socket/socket.scm")

(define txt-str-prev)

; loop on ghost
(while #t
	(let ((txt-str "") (ghost-result '()))
		(set! ghost-result (ghost-get-result))
		(for-each (lambda (a)
			(set! txt-str (string-trim (string-append txt-str " " (cog-name a)))))
			ghost-result
		)
		(if (equal? txt-str txt-str-prev)
			(continue)
			(begin 
				(act-say txt-str)
				(set! txt-str-prev txt-str)
			)
		)
		(usleep 100000)
	)
)

