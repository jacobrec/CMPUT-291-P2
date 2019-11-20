#lang racket
(require readline)
(require readline/readline)
(require "globals.rkt")
(require "ffi_interface.rkt")
(require "parser.rkt")
(require "queries.rkt")

(define full-output #f)

(define (process-command cmd)
  (case (car cmd)
    [(mode) (change-mode (second cmd))]
    [(query) (do-query (drop cmd 1) full-output)]))

(define (change-mode mode)
  (case mode
    [(full) (set! full-output #t)]
    [(brief) (set! full-output #f)]))

(define (main)
  (define input (readline "> "))
  (unless (or (eof-object? input) (string=? input "exit"))
    (add-history input)
    (define cmd (command input))
    (if (or (eq? 'fail (car cmd))
            (not (string=? "" (cadr cmd))))
      (displayln "invalid command")
      (process-command (car cmd)))
    (main))
  (when (eof-object? input)
    (displayln "")))

(main)
(cleanup_databases jdb)
