#lang racket

(require "ffi_interface.rkt")
(provide do-query)

(define (optimize queries)
  queries)

(define (do-query querys full-output)
  (set! querys (optimize querys))
  (displayln querys)
  (define resultset (emptyset))
  (for ([q querys])
    (cond
      [(eq? (car q) 'term)
       (set! resultset
         (query-term jdb resultset (caddr q) (cadr q) (cadddr q)))]
      [(eq? (car q) 'email)
       (set! resultset
         (query-email jdb resultset (caddr q) (cadr q)))]))
  (display_set jdb resultset #t))
