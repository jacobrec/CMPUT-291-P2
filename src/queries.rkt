#lang racket

(require "ffi_interface.rkt")
(provide do-query)

(define (optimize queries)
  ;; TODO: add optimizations
  ; Ideas:
  ; - Within terms, remove total subsets. ("ga" "subj:ga") -> ("ga")
  ;                                       ("ga%" "ga") -> ("ga%")
  ; - Change email queries to check other email fields in the c before adding to set?
  ; - Change all date into a single date range
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
         (query-email jdb resultset (caddr q) (cadr q)))]
      [(eq? (car q) 'date)
       (displayln "Date queries not yet supported")]))
  (display_set jdb resultset #t))
