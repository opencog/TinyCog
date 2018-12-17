;; Send Commands to Professor Einstein 
;;
;; File: cmd_einstein.scm
;; Author: Dagim Sisay <dagiopia@gmail.com>
;; Date: November, 2018
;; License: AGPL

(use-modules (ice-9 hash-table)
             (json))


; XXX there should be error checks here... just have to make sure the robot has its
;     port 8080 open before starting this script. Don't know how to do the error checks
;     in the time I have now. 
(define sck (socket PF_INET SOCK_STREAM 0))
;; // XXX uncomment the following line and the one in the function send-to-einstein
;(connect sck AF_INET (inet-pton AF_INET "192.168.1.1") 8080)

; whwn the robot remains idle for sometime, it shuts down.
; to avoid that, we're gonna have a thread that sends
; the robot random commands like turning the head and
; sticking his toungue out every few minutes just to keep
; it alive.
; Keeping the random actions to something that doesn't block
; the loop for long is preferable since stoping is only
; possible throught the do-random-actions variable,
; the loop should go around and check the value of that variable
; as frequently as possible.
(define-public do-random-actions #t)

; pause the random actions thread when sending a command
(define-public (send-cmd CMD)
	(set! do-random-actions #f)
	(send-to-einstein CMD)
	(set! do-random-actions #t)
)


; the command codes
(define einstein-cmds (make-hash-table))
(hash-set! einstein-cmds "head_right" "<MO=HT,1,0.5>")
(hash-set! einstein-cmds "head_center" "<MO=HT,0.5,0.5>")
(hash-set! einstein-cmds "head_left" "<MO=HT,0,0.5>")
(hash-set! einstein-cmds "head_up" "<MO=HN,1,0.5>")
(hash-set! einstein-cmds "head_down" "<MO=HN,0,0.5>")
(hash-set! einstein-cmds "mouth_open" "<MO=MO,1,0.5>")
(hash-set! einstein-cmds "mouth_close" "<MO=MO,0,0.5>")
(hash-set! einstein-cmds "eyebrow_up" "<MO=EB,1,0.5>")
(hash-set! einstein-cmds "eyebrow_down" "<MO=EB,0,0.5>")
(hash-set! einstein-cmds "eyelid_open" "<MO=EL,0,0.5>")
(hash-set! einstein-cmds "eyelid_close" "<MO=EL,1,0.5>")
(hash-set! einstein-cmds "cheek_up" "<MO=CH,0,0.5>")
(hash-set! einstein-cmds "cheek_normal" "<MO=CH,0.5,0.5>")
(hash-set! einstein-cmds "cheek_down" "<MO=CH,1,0.5>")
(hash-set! einstein-cmds "arm_up" "<MO=AR,1,0.5>")
(hash-set! einstein-cmds "arm_down" "<MO=AR,0,0.5>")

; list cmds
(define cmds (list "head_right" "head_left" "head_up" "head_down"
                   "mouth_open" "mouth_close" "eyebrow_up" "eyebrow_down"
                   "eyelid_open" "eyelid_close" "cheek_up" "cheek_down"))


; function to encode commands to the einstein robot.
(define-public (cmd-to-einstein cmd)
	(define fcmd (scm->json-string `(("data" . (("output" ,@cmd))) ("cmd" . "activity.recieved"))))
	(string-append (format #f "~5,'0d" (string-length fcmd)) fcmd)
)


; send einstein command through a socket to port 8080 which is the default for the robot
(define-public (send-to-einstein STR)
   ;(display (cmd-to-einstein STR) sck)
	(display (cmd-to-einstein STR)) (newline)
)

; just an intro
(send-to-einstein "Hello, I'm Dr Robot o")

; seconds to microseconds
(define (us s) (inexact->exact (floor (* s 1000000 ))))

; send a command CMD and delay for SEC seconds
(define (send-delay CMD SEC) (send-to-einstein (hash-ref einstein-cmds CMD)) (usleep (us SEC)))

; send a command CMD and delay for a random number of seconds less than 10
(define (send-rnd-delay CMD) 
	(send-to-einstein (hash-ref einstein-cmds CMD)) 
	(usleep (us (random 10)))
)

;; Define Routine acts

(define-public (act-head-down) (send-to-einstein (hash-ref einstein-cmds "head_down")))
(define-public (act-head-up) (send-to-einstein (hash-ref einstein-cmds "head_up")))
(define-public (act-head-center) (send-to-einstein (hash-ref einstein-cmds "head_center")))
(define-public (act-eyes-open) (send-to-einstein (hash-ref einstein-cmds "eyelid_open")))
(define-public (act-eyes-close) (send-to-einstein (hash-ref einstein-cmds "eyelid_close")))
(define-public (act-arm-up) (send-to-einstein (hash-ref einstein-cmds "arm_up")))
(define-public (act-arm-down) (send-to-einstein (hash-ref einstein-cmds "arm_down")))


; Shake head horizontally - nod no
; XXX this routine is no good look into increasing the speed of the motor esti
(define-public (act-head-hshake)
	(define act (list "head_left" "head_right" "head_left" "head_right"
	                  "head_left" "head_right" "head_center"))
	(define del (list 0.4 0.4 0.4 0.4 0.4 0.4 0.4))
	(map send-delay act del)
)

; Shake head vertically - nod yes
(define-public (act-head-vshake)
	(define act (list "head_up" "head_down" "head_up" "head_down" "head_up"))
	(define del (list 0.4 0.4 0.4 0.4 0.4))
	(map send-delay act del)
)

(define-public (look-side-to-side)
   (define act (list "head_center" "head_right" "head_left" "head_center"))
	(define del (list 0.1 3 3 0.1))
	(map send-delay act del)
)

; smile for a few seconds and then back to normal
(define-public (act-smile)
	(define act (list "cheek_normal" "cheek_up" "cheek_normal"))
	(define del (list (random 3) (random 5) 1))
	(map send-delay act del)
)

; show a sad face for a little while
(define-public (act-sad)
	(define act (list "cheek_normal" "cheek_down" "cheek_normal"))
	(define del (list (random 3) (random 5) 1))
	(map send-delay act del)
)


; show an afraid face for a little while
(define-public (act-silly)
	(define act (list "cheek_normal" "cheek_up" "mouth_open" "mouth_close" "cheek_normal"))
	(define del (list 2 1 (random 5) 1 2 ))
	(map send-delay act del)
)

; pan the head to a given point
(define-public (act-pan-head POS)
	(if (bn-zero-one? POS)
		(begin
			(send-to-einstein (format #f "<MO=HT,~f,0.5>" POS))
			(stv 1 1)
		)
		(stv 0 1)
	)
)

; tilt the head to a given point
(define-public (act-tilt-head POS)
	(if (bn-zero-one? POS)
		(begin
			(send-to-einstein (format #f "<MO=HN,~f,0.5>" POS))
			(stv 1 1)
		)
		(stv 0 1)
	)
)



(define-public (random-cmds)
	(while #t
		(if do-random-actions
			(act-eyes-open) ; change this to something more relevant
			(continue)
		)
		(usleep (us (random 30))) ; delay random no of seconds less than 30s
		                          ; XXX check if the exact value should be 30s
	)
)

; reset the prof to normal posture after
; internal startup script made him abnormal
(arm-down) (usleep (us 1))
(head-up) (usleep (us 1))
(head-center) (usleep (us 1))
(eyes-open) (usleep (us 1))

; send random commands to einstein on a new thread so that it doesn't 
; go to sleep. When a speech or intentional command is to be sent, 
; do-random-actions should be set to false so that no interfering cmnds
; are sent during that time and then reenabled again. 
(call-with-new-thread random-cmds)
