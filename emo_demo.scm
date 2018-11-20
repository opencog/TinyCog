; guile module for emotion recognition demo

(use-modules (ice-9 hash-table))

(setlocale LC_CTYPE "C")
(define TOPDIR (getcwd))

(add-to-load-path "/usr/local/share/opencog")
(add-to-load-path TOPDIR)

(load-extension "build/libemo_demo" "init_emo_demo")

; utility
(define t (stv 1 1))
(define f (stv 0 1))

(define emo_types (make-hash-table))
(hash-set! emo_types "anger" "angry")
(hash-set! emo_types "disgust" "disgusted")
(hash-set! emo_types "fear" "fearful")
(hash-set! emo_types "happy" "happy")
(hash-set! emo_types "sad" "sad")
(hash-set! emo_types "surprise" "surprised")
(hash-set! emo_types "neutral" "normal")

; functions for ghost
(define-public (func-see-face) 
	(if (not 
			(equal? 
				(car (string-split (det-emo) #\,)) 
				""
			)
		) 
		(ListLink (WordNode "Yes,")
		          (WordNode "I")
					 (WordNode "can")
		)
		(ListLink (WordNode "No,")
		          (WordNode "I")
					 (WordNode "can")
					 (WordNode "not")
		)
	)
)

; anger, disgust, fear, happy, sad, surprise, neutral
(define-public (func-face-emotion) 
	(define emo_ (car (string-split (det-emo) #\,)))
	(if (equal? emo_ "")
		 (ListLink (WordNode "I")
		           (WordNode "can't")
		           (WordNode "actually")
		           (WordNode "see")
		           (WordNode "you")
		           (WordNode "now.")
		           (WordNode "could")
		           (WordNode "you")
		           (WordNode "maybe")
		           (WordNode "move")
		           (WordNode "closer")
		 )
		 (ListLink (WordNode "I")
		           (WordNode "think")
		           (WordNode "you")
	              (WordNode "look")
		           (WordNode (hash-ref emo_types emo_))
		 )
	)
)


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
