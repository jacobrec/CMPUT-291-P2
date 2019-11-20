#lang racket

(require "ffi_interface.rkt")
(provide do-query)

(define (do-query querys full-output)
  (define resultset (emptyset))
  (for ([q querys])
    (cond
      [(eq? (car q) 'term)
       (set! resultset
         (query-term jdb resultset (caddr q) (cadr q) (cadddr q)))]))
  (display_set jdb resultset #t))
