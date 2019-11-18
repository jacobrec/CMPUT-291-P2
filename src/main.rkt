#lang racket
(require "globals.rkt")
(require "ffi_interface.rkt")
(require "parser.rkt")

(define full-output #f)

(define (process-command cmd)
  (displayln cmd))

(define (main)
  (display "> ")
  (define input (read-line))
  (unless (or (eof-object? input) (string=? input "exit"))
    (define cmd (command input))
    (if (or (eq? 'fail (car cmd))
            (non-empty-string? (cdr cmd)))
      (displayln "invalid command")
      (process-command (car cmd)))
    (main))
  (when (eof-object? input)
    (displayln "")))

(main)
