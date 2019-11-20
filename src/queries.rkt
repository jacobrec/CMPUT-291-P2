#lang racket

(require "ffi_interface.rkt")
(provide do-query)

;; TODO:
; Test this througally for edge conditions
(define (string-min s1 s2)
  (if (string<? s1 s2) s1 s2))
(define (string-max s1 s2)
  (if (string>? s1 s2) s1 s2))

(define (date->num date)
  (define items (string-split date "/"))
  (+
    (* 100 100 (string->number (list-ref items 0)))
    (* 100 (string->number (list-ref items 1)))
    (string->number (list-ref items 2))))
(define (num->date datenum)
  (when (< datenum 0)
    (set! datenum 0))
  (when (> datenum 99999999)
    (set! datenum 99999999))
  (define year (number->string (quotient datenum 10000)))
  (define month (number->string (remainder (quotient datenum 100) 100)))
  (define day (number->string (remainder datenum 100)))
  (set! year (string-append (make-string (- 4 (string-length year)) #\0) year))
  (set! month (string-append (make-string (- 2 (string-length month)) #\0) month))
  (set! day (string-append (make-string (- 2 (string-length day)) #\0) day))
  (string-append year "/" month "/" day))
(define (inc-date date)
  (num->date (add1 (date->num date))))
(define (dec-date date)
  (num->date (sub1 (date->num date))))


(define (compress-dates queries)
  (define without-dates (filter
                          (λ (q)
                             (not (eq? (car q) 'date)))
                          queries))
  (define dates (filter
                  (λ (q)
                     (eq? (car q) 'date))
                  queries))

  ;; This could probably be done simpler if the dates were converted to numbers
  ; first and processed as numbers, then converted to strings at the end
  (define (remove-dates)
    (define min-date "0000/00/00")
    (define max-date "9999/99/99")
    (define over #f)
    (for [(q dates)]
      (cond
        [(string=? (cadr q) "<")
         (when (string=? (caddr q) "0000/00/00") (set! over #t))
         (set! max-date (string-min max-date (caddr q)))]
        [(string=? (cadr q) ">")
         (when (string=? (caddr q) "9999/99/99") (set! over #t))
         (set! min-date (string-max min-date (inc-date (caddr q))))]
        [(string=? (cadr q) ":")
         (set! min-date (string-max min-date (caddr q)))
         (set! max-date (string-min max-date (inc-date (caddr q))))]
        [(string=? (cadr q) "<=")
         (set! max-date (string-min max-date (inc-date (caddr q))))]
        [(string=? (cadr q) ">=")
         (set! min-date (string-max min-date (caddr q)))]))
    (cond
      [(or over (string<? max-date min-date)) '((none))]
      [(and (string=? min-date "0000/00/00")
            (string=? max-date "9999/99/99")
            (empty? without-dates) '((date "0000/00/00" "9999/99/99")))]
      [(and (string=? min-date "0000/00/00")
            (string=? max-date "9999/99/99")) without-dates]
      [else (cons (list 'date min-date max-date) without-dates)]))
  (if (empty? dates)
    queries
    (remove-dates)))


(define (optimize queries)
  ;; TODO: add optimizations
  ; Ideas:
  ; - Within terms, remove total subsets. ("ga" "subj:ga") -> ("ga")
  ;                                       ("ga%" "ga") -> ("ga%")
  (set! queries (compress-dates queries))
  queries)

(define (do-query querys full-output)
  (set! querys (optimize querys))
  (define resultset (emptyset))
  (displayln querys)
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
