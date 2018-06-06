;; File: behavior.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018

(use-modules
	(opencog)
	(opencog exec)
	(opencog atom-types)
	(opencog pointmem)
	(opencog nlp relex2logic)
	(opencog openpsi)
	(opencog openpsi dynamics)
	(opencog eva-behavior)
	(opencog nlp )
	(opencog ghost))


;; ---------------------------------------------
;; Define functions used by the rules
(include "functions.scm")


;; ----------------------------------------------
;; Parse ghost topic files. 
;; Ghost rules handle speech interactions.

;(ghost-parse-file (string-append TOPDIR "behavior/" "test.top"))
(ghost-parse-file (string-append TOPDIR "behavior/" "demo.top"))


;; ----------------------------------------------
;; Add events and callbacks.
;; Events are monitored actions the robot periodically checks for.
;; The detection of an event causes interaction rules to trigger.
;; The interaction rules can then in turn cause ghost rules to trigger speech output
;; or cause some other function to execute.

(include "events.scm")


;; ----------------------------------------------
;; Add dynamic interaction rules.
;; Interaction rules mainly handle non-speech triggered response.

(include "interaction-rules.scm")

;; ------------------------------------------------
;; Add testing functions
;; The functions are used to simulate the occurence of events for testing.

;(include "test.scm")
(include "updater.scm")


;; Run ghost loop
(ghost-run)


;; Run dynamic psi
(psi-updater-run)
