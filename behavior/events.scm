;; File: events.scm
;; ;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018

;;; -------------------------------------------------
;;; This file contains all events and their callbacks
;;; -------------------------------------------------


;; --------------------------------------------------
;; Create events

; Face detection event
(define-public event-face (psi-create-monitored-event "face-detected?"))


;; ----------------------------------------------------
;; Create event monitoring functions

; Monitor face detection
; Currently this checks a global variable
; FIXME: Need to query the atomspace instead
(define-public (monitor-face-detection)
	(if (and (defined? 'global-face) (= (hash-ref global-face 'val) 1))
		(begin
			(psi-set-event-occurrence! event-face)
			(hash-set! global-face 'val 0)
		)
	)
)



;; ----------------------------------------------------
;; Register the monitoring functions here

(psi-set-event-callback! monitor-face-detection)
