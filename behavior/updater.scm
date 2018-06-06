;; File: updater.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: June, 2018

;;; -----------------------------------------------------------------------------
;;; This file contains temporary code that updates global variables for detectors
;;; for demo purpose
;;; -----------------------------------------------------------------------------

; Make global variables
(define-public global-face (make-hash-table))
(hash-set! global-face 'update-time -100)

(define-public global-smile (make-hash-table))
(hash-set! global-smile 'update-time -100)

(define-public global-salient (make-hash-table))
(hash-set! global-salient 'update-time -100)

(define (updater)

	(define txt-str-prev "")

	(while #t
	 	(let ((ret-face (det-face))
		      (ret-smile (det-face-smile))
		      (ret-salient (det-salient-point))
		      (txt-str "")
		      (ghost-result '()))

		(if (not (string-null? ret-face))
			(begin
			    (hash-set! global-face 'faces (string-split ret-face #\;))
			    (hash-set! global-face 'update-time (current-time))
			)
		)

		
		(if (not (string-null? ret-smile))
			(begin
			    ; For the time being only detect smile for the first face
			    (let* ((faces (string-split ret-smile #\;))
			    	   (face (list-ref faces 0))
				   (smile (list-ref (string-split face #\,) 3)))

				   (if (equal? smile "true")
				   	(begin
					   (hash-set! global-smile 'face face)
					   (hash-set! global-smile 'update-time (current-time))
					)
				   )
			     )
			)
		)

		(if (not (string-null? ret-salient))
			(begin
				(hash-set! global-salient 'loc (string-split ret-salient #\,))
				(hash-set! global-salient 'update-time (current-time))
			)
		)
		
		; Finally send text output to stt when there is one
		; Note: This code is only for demo (stt input should be read from port)
       
		(set! ghost-result (ghost-get-result))
		(for-each (lambda (a)
		    (set! txt-str (string-trim (string-append txt-str " " (cog-name a))))
		    )ghost-result
		)

		(if (equal? txt-str txt-str-prev)
			(continue)
			(begin
				(act-say txt-str)
				(set! txt-str-prev txt-str)
			)

		)

)


; Start the update function in a thread

(define update-thread (call-with-new-thread updater))
