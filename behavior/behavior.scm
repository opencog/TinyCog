;; File: behavior.scm
;; Author: Eskender Besrat <eskenderbesrat@gmail.com>
;; License: AGPL
;; Date: May, 2018


;; ---------------------------------------------
;; Define functions used by the rules
(include "functions.scm")

;; Load ghost scripts
(begin (display "Loading Ghost Scripts...") (newline))
(ghost-parse-file (string-append TOPDIR "/ghost/concepts_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/character_1.ghost"))
(ghost-parse-file (string-append TOPDIR "/ghost/intro1.ghost"))
(begin (display "Done Loading Ghost Scripts.") (newline))


;; Run ghost loop
(ghost-run)


;; Run dynamic psi
;(psi-updater-run)
