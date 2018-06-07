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

; Smile detection event
(define-public event-smile (psi-create-monitored-event "smile-detected?"))

; Saliency detection event
(define-public event-saliency (psi-create-monitored-event "saliency?"))


;; ----------------------------------------------------
;; Create event monitoring functions
;; Currently the functions check global variables updated from C++ detectors
;; FIXME: Need to query the atomspace instead


; Monitor face detection in 10 sec interval (for the time being)
(define face-last (current-time))

(define-public (monitor-face-detection)
	(if (and (defined? 'global-face) (< (- (current-time) (hash-ref global-face 'update-time)) 1))
		;(> (- (current-time) face-last) 10))
			(begin
				(psi-set-event-occurrence! event-face)
				(set! face-last (current-time))
			)
	)
)


; Monitor smile
(define-public (monitor-smile-detection)
	(if (and (defined? 'global-smile) (< (- (current-time) (hash-ref global-smile 'update-time)) 2))
		(psi-set-event-occurrence! event-smile)



; Monitor saliency
(define salient-last (current-time))

(define-public (monitor-saliency)
	(if (and (defined? 'global-salient) (< (- (current-time) (hash-ref global-salient 'update-time)) 5)
		(> (- (current-time) salient-last) 10))
			(begin
				(psi-set-event-occurrence! event-saliency)
				(set! salient-last (current-time))
			)
	)
)





;; ----------------------------------------------------
;; Register the monitoring functions here

(psi-set-event-callback! monitor-face-detection)
(psi-set-event-callback! monitor-smile-detection)
(psi-set-event-callback! monitor-saliency)
