#lang racket

(require "ffi_interface.rkt")
(provide do-query)

;; TODO:
; Test this througally for edge conditions
(define (string-min s1 s2)
  (if (string<? s1 s2) s1 s2))
(define (string-max s1 s2)
  (if (string>? s1 s2) s1 s2))
(define (inc-date date)
  date) ;; TODO: 1999/99/99  or 1999/12/31 -> 2000/00/00
        ;; depending on how discussion forum question is answered
(define (dec-date date)
  date) ;; TODO: 2000/00/00 -> 1999/99/99 or 1999/12/31
        ;; depending on how discussion forum question is answered
(define (compress-dates queries)
  (define without-dates (filter
                          (λ (q)
                             (not (eq? (car q) 'date)))
                          queries))
  (define dates (filter
                  (λ (q)
                     (eq? (car q) 'date))
                  queries))

  (define (remove-dates)
    (define min-date "0000/00/00")
    (define max-date "9999/99/99")
    (for [(q dates)]
      (cond
        [(string=? (cadr q) "<")
         (set! max-date (string-min max-date (caddr q)))]
        [(string=? (cadr q) ">")
         (set! min-date (string-max min-date (dec-date (caddr q))))]
        [(string=? (cadr q) ":")
         (set! min-date (string-max min-date (caddr q)))
         (set! max-date (string-min (inc-date max-date) (caddr q)))]
        [(string=? (cadr q) "<=")
         (set! max-date (string-min max-date (inc-date (caddr q))))]
        [(string=? (cadr q) ">=")
         (set! min-date (string-max min-date (caddr q)))]))
    (if (string<? max-date min-date)
      (list)
      (cons (list 'date min-date max-date) without-dates)))
  (if (empty? dates)
    queries
    (remove-dates)))


(define (optimize queries)
  ;; TODO: add optimizations
  ; Ideas:
  ; - Within terms, remove total subsets. ("ga" "subj:ga") -> ("ga")
  ;                                       ("ga%" "ga") -> ("ga%")
  ; - Change email queries to check other email fields in the c before adding to set?
  (set! queries (compress-dates queries))
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
       (set! resultset
         (query-date jdb resultset (cadr q) (caddr q)))]))
  (display_set jdb resultset #t))
