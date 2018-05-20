;; File: scratch.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018

;;; This is a web server implementation for listening and responding to scratch request
;;; Server runs on port 7777

(use-modules (web server)
 	     (web request)
	     (web response)
	     (web uri))


;; Functons used by http handler implemented below

; Input speech taking function
(define (speech-input speech)

	(set! speech (string-trim-both speech))

	(if (not (string-null? speech))

		; test-ghost is used for the time being
		(test-ghost speech)
	)
	"Ok" ;Http response
)


; Input rule taking function 
(define (ghost-rule rule)

	(ghost-parse rule)
	"Ok"	;HTTP response
)

; Function for speech output
(define (speech-output)
	
	;(string-append "speechOutput " "Dummy output\n")

	(define txt-str "")
	(define result (ghost-get-result))
	
	(for-each (lambda(a)
		(set! txt-str (string-trim (string-append txt-str " " (cog-name a))))
		)
		result
	)

	(string-append "speechOutput " txt-str "\n")
)

; Function for sending cross domain to scratch
(define (send-cross-domain-policy)
	(string-append "<cross-domain-policy>"
				"<allow-access-from-domain=\"*\" to-ports=\"<7777>\"/>"
			"</cross-domain-policy>"
	)
)


; Function that executes when scratch program starts (green flag clicked)
; Does nothing for the time being.
(define (reset-all)
	"Ok"	
)




; Implement HTTP Handler
(define (scratch-handler request request-body)
	
	; Split string into list by '/' character
	;(define command (string-split request #\/))

	(define command
			(split-and-decode-uri-path (uri-path (request-uri request))))


	(define resp
		(cond
			((equal? (list-ref command 0) "speechInput") (speech-input (list-ref command 1)))
			((equal? (list-ref command 0) "ghostRule") (ghost-rule (list-ref command 1))  )
			((equal? (list-ref command 0) "poll") (speech-output) )
			((equal? (list-ref command 0) "crossdomain.xml") (send-cross-domain-policy))
			((equal? (list-ref command 0) "reset_all") (reset-all))
		)
	)

	;(display command)
	;(newline)
	
	(values 
		'((content-type . (text/html)))
		resp
	)	
)	



; Run the server in a thread
(call-with-new-thread
	(lambda ()
		(run-server scratch-handler 'http '(#:port 7777))
	)
)
(display "Scratch server started on port 7777.\n")
