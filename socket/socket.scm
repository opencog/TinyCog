;; File: socket.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018

;;; This file contains TCP socket implementation for input and output text from and to STT and TTS respectively

(use-modules (ice-9 textual-ports))


;; Port 5555: Text input from STT
(define input-port 5555)

;; Port 6666: Text output to TTS 
(define output-port 6666)



(define (input-from-stt)
	(let ((sock (socket PF_INET SOCK_STREAM IPPROTO_TCP))
	      (speech-txt ""))

		(bind sock (make-socket-address AF_INET INADDR_LOOPBACK input-port))
		(listen sock 2)

		(simple-format #t "INPUT: Listening for text input from port ~S. " input-port)
		(newline)

		(while #t
			(let* ((client-conn (accept sock))
			       (client-details (cdr client-conn))
			       (client (car client-conn)))

				(simple-format #t "INPUT: Got client connection: ~S" client-details)
				(newline)
				(simple-format #t "INPUT: Client address: ~S" (gethostbyaddr 
									  (sockaddr:addr client-details)))
				(newline)
				(while #t
					(display "speech: " client)
					(set! speech-txt (get-line client))
					(if (eof-object? speech-txt)
					    (break)
					    (begin 
						; Remove any trailing \r\n 
						(set! speech-txt (string-trim-right speech-txt #\newline))
						(set! speech-txt (string-trim-right speech-txt #\return))
						(set! speech-txt (string-trim-both speech-txt))

						; Finally give txt to Ghost
						(if (string-null? speech-txt)
						    (begin (display "Empty string\n" client) (continue))
						    ; test-ghost used for the time being 
						    (test-ghost speech-txt)
						)
					    )
					)
					(begin (display (ghost-get-result)) (newline))
				)
				(close client)
				(display "INPUT: Client closed.\n")
			)
		)
	)
)



(define (output-to-tts)
	
	(let ((sock (socket PF_INET SOCK_STREAM IPPROTO_TCP))
	      (txt-str "")
	      (txt-str-prev "")
	      (ghost-result '())
	      (n 0))

		(bind sock (make-socket-address AF_INET INADDR_LOOPBACK output-port))
		(listen sock 1)

		(simple-format #t "OUTPUT: Making text output available to port ~S. " output-port)
		(newline)


		(while #t
			(let* ((client-conn (accept sock))
			       (client-details (cdr client-conn))
			       (client (car client-conn)))

				(simple-format #t "OUTPUT: Got client connection: ~S" client-details)
				(newline)
				(simple-format #t "OUTPUT: Client address: ~S" (gethostbyaddr 
									  (sockaddr:addr client-details)))
				(newline)
			
	
				(catch #t

				(lambda ()
				(while #t
				  (set! ghost-result (ghost-get-result))
				  ;(display "test") 
				  (set! txt-str "")
				  (for-each (lambda(a)
				     (set! txt-str (string-trim (string-append txt-str " " (cog-name a))))
				     )ghost-result
				  )
				
				  (if (equal? txt-str txt-str-prev)
					(continue)
					(begin
						;FIXME: when port's other end is closed
						(display (string-append txt-str "\n") client)
						(set! txt-str-prev txt-str)
						(force-output client)
					)	
				  )
			       ))
				(lambda (key . args)
				(display "OUTPUT: Client closed.\n")
				(close client))
				)
			)
			(usleep 100000)
		)
	)
)

(
define (output-to-einstein)
	(define txt-prev "")
	(define txt-curr "")
	(while #t 
		(set! txt-curr (map cog-name (ghost-get-result)))
		(set! txt-curr (string-concatenate (map append-space txt-curr)))
		(if (or (equal? txt-prev txt-curr) (equal? txt-curr ""))
			(continue)
			(begin 
				;(act-say txt-curr) ; send directly to speaker ... but we don't need this now. 
				(send-to-einstein txt-curr) ; we need this one now.
				(set! txt-prev txt-curr)
			)
		)
		(usleep 100000) ; 100ms
	)
)


;; Instantiate functions in threads

(define input-thread (call-with-new-thread input-from-stt))
(display "Text input thread started.\n")


;(define output-thread (call-with-new-thread output-to-tts))   ; we don't need this now. 
(call-with-new-thread output-to-einstein)
(display "Text output thread started.\n")

