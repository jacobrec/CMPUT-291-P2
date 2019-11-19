#lang racket
(provide (all-defined-out))
(require ffi/unsafe
         ffi/unsafe/define)

(define libdb (ffi-lib "libdb"))
(define-ffi-definer define-jbdb (ffi-lib "./jbdblib"))

(define-jbdb query (_fun -> _int))

(query)
