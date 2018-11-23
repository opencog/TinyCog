; guile module for emotion recognition demo

; import utility modules
(use-modules (ice-9 hash-table)
             (ice-9 format)
				 (json))


(setlocale LC_CTYPE "C")
(define TOPDIR (getcwd))

(add-to-load-path "/usr/local/share/opencog")
(add-to-load-path TOPDIR)

; import opencog modules
(use-modules (opencog)
             (opencog nlp)
             (opencog nlp relex2logic)
             (opencog openpsi)
             (opencog ghost)
             (opencog ghost procedures))


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

; check last executed ghost rule
;   The point of these is that after calling (ghost TXT) or (test-ghost TXT)
;   the response is caught using (ghost-get-result) which could still hold
;   the response from the last rule. This could easily be checked by saving 
;   the last response to another variable and waiting for the data from 
;   (ghost-get-result) to change (like we did before). However, doing this 
;   has a severe disadvantage where we have a rule with the ^keep function
;   or two different rules that might responde with the same text. The 
;   comparision would suggest that the last reponse haven't yet changed and 
;   the totally legitimate response goes unused. 
; 
;   To aliviate this problem, I modified the ghost src ghost/test.scm
;   changed L60-62. The change is: the ghost-result variable is set to
;   empty set once the (ghost-get-result) function returns the response.
;   Thus all subsequent calls would only receive ()
;   Even if the same rule is triggred again, since the (ghost-get-result) 
;   function was empty first, any change can tell this program that it's a new
;   response which needs to be said out loud. 
; 
;   I think a much better method than this is to have a public function
;   that gives the value of the anchor ghost-last-executed... but this 
;   only tells the last rule that was executed. If the rule has a ^keep
;   and gets executed again, that value remains the same. But something like that... 


; function to encode commands to the einstein robot into json and then format 
; it approprietly so that it's acceptable
(define-public (cmd-to-einstein cmd)
	(define x 
		(scm->json-string `(("data" . (("output" ,@cmd))) ("cmd" . "activity.recieved")))
	)
	(string-append (format #f "~5,'0d" (string-length x)) x)
)

; send text through a socket -> this is intended to serve in sending to the prof
; einstein robot. 
; XXX there should be error checks here... just have to make sure the robot has its
;     port 8080 open before starting this script. Don't know how to do the error checks
;     in the time I have now. 
(define sck (socket PF_INET SOCK_STREAM 0))
(connect sck AF_INET (inet-pton AF_INET "192.168.1.1") 8080)
(define-public (send-to-einstein STR)
	(display (cmd-to-einstein STR) sck)
)

(begin (display "Loading Ghost Scripts...") (newline))
;(ghost-parse-file (string-append TOPDIR "/ghost/concepts.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character1.ghost"))
(begin (display "Done Loading Ghost Scriptas") (newline))

(include "socket/socket.scm")
