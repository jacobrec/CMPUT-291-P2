#lang racket
(provide (all-defined-out))
(require ffi/unsafe
         ffi/unsafe/define)

(provide emptyset
  query-term
  setup_databases
  cleanup_databases)

; struct set {
;   int* buckets;
;   int bucketLength;
;   int size;
;   bool isUsed;
; }
(define-cstruct _set ([buckets      (_ptr io _int)]
                      [bucketLength _int]
                      [size         _int]
                      [isUsed       _bool]))

(define libdb (ffi-lib "libdb"))
(define-ffi-definer define-jbdb (ffi-lib "./jbdblib"))

(define-jbdb setup_databases (_fun -> _pointer))
(define-jbdb queryTerm (_fun _pointer (_ptr io _set) _string _int _bool
                             -> (_ptr io _set)))
(define-jbdb queryTerm2
             (_fun _pointer (_ptr io _set) _string _string _int _bool
                   -> (_ptr io _set)))
(define-jbdb display_set (_fun _pointer (_ptr io _set) _bool -> _void))
(define-jbdb emptyset (_fun -> (_ptr io _set)))
(define-jbdb cleanup_databases (_fun _pointer -> _void))

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


;; Tests
(define jdb (setup_databases))
(define resultset (emptyset))
(set! resultset (query-term jdb resultset "ga" 'both #t))
(set! resultset (query-term jdb resultset "ga" 'subj #t))
(display_set jdb resultset #t)
(cleanup_databases jdb)
