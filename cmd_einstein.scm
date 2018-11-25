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
;(connect sck AF_INET (inet-pton AF_INET "192.168.1.1") 8080)

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

; whwn the robot remains idle for sometime, it shuts down. 
; to avoid that, we're gonna have a thread that sends 
; the robot random commands like turning the head and 
; sticking his toungue out every few minutes just to keep 
; it alive

(define (us s) (* s 1000000 ))

(define (send-delay CMD SEC) (send-to-einstein (hash-ref einstein-cmds CMD)) (usleep (us SEC)))
(define (send-rnd-delay CMD) 
	(send-to-einstein (hash-ref einstein-cmds CMD)) 
	(usleep (us (random 10)))
)

(define-public (routine-1)
	(define act (list "head_center" "head_right" "head_left" "head_center"))
	(define del (list (random 20) (random 10) (random 10) (random 10)))
	(map send-delay act del)
)


(define-public (routine-1.1)
	(define act (list "head_center" "head_left" "head_right" "head_center"))
	(define del (list (random 20) (random 10) (random 10) (random 10)))
	(map send-delay act del)
)


(define-public (routine-2)
	(define act (list "mouth_open" "mouth_close"))
	(define del (list 2 2))
	(map send-delay act del)
)


; smile for a few seconds and then back to normal
(define-public (routine-smile)
	(define act (list "cheek_normal" "cheek_up" "cheek_normal"))
	(define del (list (random 10) (random 10) (random 10)))
	(map send-delay act del)
)

; show a sad face for a little while
(define-public (routine-sad)
	(define act (list "cheek_normal" "cheek_down" "cheek_normal"))
	(define del (list (random 10) (random 10) (random 10)))
	(map send-delay act del)
)


; show an afraid face for a little while
(define-public (routine-silly)
	(define act (list "cheek_normal" "cheek_up" "mouth_open" "mouth_close" "cheek_normal"))
	(define del (list (random 10) 1 3 (random 10) (random 10) ))
	(map send-delay act del)
)

(define-public do-random-actions #t)

(define-public (random-cmds)
	(while #t
		(if do-random-actions
			((usleep (us 5))
			(routine-1)
			(usleep (us 5))
			(routine-2)
			(usleep (us 5))
			(routine-1.1)
			(usleep (us 5))
			(routine-smile)
			(usleep (us 5))
			(routine-sad)
			(usleep (us 5))
			(routine-1)
			(map send-rnd-delay cmds)
			(usleep (us 5))
			(routine-1.1)
			(usleep (us 5))
			(routine-smile)
			(usleep (us 5))
			(routine-silly)
			(usleep (us 5))
			(routine-1.1)
			(usleep (us 5))
			(routine-1)
			(usleep (us 20)))

			(continue)
		)
	)
)

(call-with-new-thread random-cmds)
