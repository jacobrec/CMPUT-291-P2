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
(define-jbdb query (_fun (_ptr io _set) _pointer -> (_ptr io _set)))
(define-jbdb display_set (_fun _pointer (_ptr io _set) _bool -> _void))
(define-jbdb emptyset (_fun -> (_ptr io _set)))
(define-jbdb cleanup_databases (_fun _pointer -> _void))

(define jdb (setup_databases))
(define resultset (emptyset))
(set! resultset (query resultset jdb))
(display_set jdb resultset #t)

(cleanup_databases jdb)
