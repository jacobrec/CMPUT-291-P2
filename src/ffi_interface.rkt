#lang racket
(provide (all-defined-out))
(require ffi/unsafe
         ffi/unsafe/define)

; struct set {
;   int* buckets;
;   int bucketLength;
;   int size;
; }
(define-cstruct _set ([buckets      (_ptr io _int)]
                      [bucketLength _int]
                      [size         _int]))

(define libdb (ffi-lib "libdb"))
(define-ffi-definer define-jbdb (ffi-lib "./jbdblib"))

(define-jbdb setup_databases (_fun -> _pointer))
(define-jbdb queryTerm (_fun _pointer (_ptr io _set) _string _int _bool
                             -> (_ptr io _set)))
(define-jbdb display_set (_fun _pointer (_ptr io _set) _bool -> _void))
(define-jbdb emptyset (_fun -> (_ptr io _set)))
(define-jbdb cleanup_databases (_fun _pointer -> _void))

(define (query-term jdb result-set term type wild?)
  (define (query r-set m-term)
    (queryTerm jdb r-set m-term (string-length m-term) wild?))
  (cond [(eq? 'both type)
         (define term-s (string-append "s-" term))
         (define term-b (string-append "b-" term))
         result-set]
        [(eq? 'subj type)
         (query result-set (string-append "s-" term))]
        [(eq? 'body type)
         (query result-set (string-append "b-" term))]))


(define jdb (setup_databases))
(define resultset (emptyset))
(set! resultset (query-term jdb resultset "not" 'subj #f))
(display_set jdb resultset #t)

(cleanup_databases jdb)
