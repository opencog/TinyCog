;;; Temporary testing functions until proper test code is prepared

;; Global variable initialized with default values
(define-public global-face (make-hash-table))
(hash-set! global-face 'val 0)


;; Testing functions to simulate face detection
(define-public (test-set-face-event)
	(hash-set! global-face 'val 1)
	(hash-set! global-face 'time (current-time))
)
