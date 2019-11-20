#lang racket
(provide (all-defined-out))
(require ffi/unsafe
         ffi/unsafe/define)

(provide jdb
  emptyset
  query-term
  query-email
  setup_databases
  cleanup_databases)


(define libdb (ffi-lib "libdb"))
(define-ffi-definer define-jbdb (ffi-lib "./jbdblib"))

(define-jbdb setup_databases (_fun -> _pointer))
(define-jbdb queryTerm (_fun _pointer _pointer _string _int _bool
                             -> _pointer))
(define-jbdb queryTerm2
             (_fun _pointer _pointer _string _string _int _bool
                   -> _pointer))
(define-jbdb display_set (_fun _pointer _pointer _bool -> _void))
(define-jbdb emptyset (_fun -> _pointer))
(define-jbdb cleanup_databases (_fun _pointer -> _void))
(define-jbdb queryEmail (_fun _pointer _pointer _string _int
                              -> _pointer))
(define-jbdb queryDate (_fun _pointer _pointer _string _string
                             -> _pointer))

(define (query-date jdb result-set rstart rend)
  (queryDate jdb result-set rstart rend))

(define (query-email jdb result-set term type)
  (define (query r-set m-term)
    (queryEmail jdb r-set m-term (string-length m-term)))
  (query result-set (string-append type "-" term)))

(define (query-term jdb result-set term type wild?)
  (define (query r-set m-term)
    (queryTerm jdb r-set m-term (string-length m-term) wild?))
  (define (query2 r-set term)
    (define m-term (string-append "s-" term))
    (define o-term (string-append "b-" term))
    (queryTerm2 jdb r-set m-term o-term (string-length m-term) wild?))
  (cond [(eq? 'both type)
         (query2 result-set term)]
        [(eq? 'subj type)
         (query result-set (string-append "s-" term))]
        [(eq? 'body type)
         (query result-set (string-append "b-" term))]))


(define jdb (setup_databases))
