#lang racket
(provide (all-defined-out))
(require ffi/unsafe
         ffi/unsafe/define)

(define-ffi-definer define-jbdb (ffi-lib "./jbdblib"))

(define-jbdb testhello (_fun _int -> _int))
