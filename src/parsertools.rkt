#lang racket

(provide (all-defined-out))

(define (fail? val)
  (equal? 'fail (first val)))

(define (parsed? val)
  (not (fail? val)))

(define (parse/lit str)
  (λ (input)
    (if (string-prefix? input str)
      (list str (substring input (string-length str)))
      (list 'fail input))))

(define (parse/and . parsers)
  (λ (input)
     (foldl (λ (elm acc)
              (if (parsed? acc)
                  (let ([result (elm (second acc))])
                    (if (parsed? result)
                      (list (append (first acc) (list (first result)))
                            (second result))
                      (list 'fail input)))
                  (list 'fail input)))
            (list empty input)
            parsers)))

(define-syntax-rule (parse/and-iw parser ...)
  (parse/and (parse/strip-whitespace parser) ...))

(define (parse/or . parsers)
  (λ (input)
     (define orval
         (ormap (λ (parser)
                  (let ([result (parser input)])
                    (if (parsed? result)
                        result
                        #f)))
                parsers))
     (if orval orval (list 'fail input))))

(define-syntax-rule (parse/or-lit lit ...)
  (parse/or (parse/lit lit) ...))

(define (parse/apply func parser)
  (λ (input)
    (define result (parser input))
    (if (parsed? result)
        (list (func (first result)) (second result))
        (list 'fail input))))

(define (parse/+ parser)
  (parse/apply
    (λ (res) (append (list (first res)) (second res)))
    (parse/and parser (parse/* parser))))

(define (parse/* parser)
  (define (many-parser input)
    (let ([result (parser input)])
      (if (parsed? result)
         (let ([next-result (many-parser (second result))])
           (list (append (list (first result))
                         (first next-result))
                 (second next-result)))
         (list '() input))))
  many-parser)

(define (parse/between pOuter pInner [pLast #f])
  (define pEnd (if pLast pLast pOuter))
  (parse/apply
    (λ (res) (second res))
    (parse/and pOuter pInner pEnd)))

(define (parse/between-iw pOuter pInner [pLast #f])
  (define pEnd (if pLast pLast pOuter))
  (parse/apply
    (λ (res) (second res))
    (parse/and-iw pOuter pInner pEnd)))

(define (parse/around pOuter pInner [pLast #f])
  (define pEnd (if pLast pLast pOuter))
  (parse/apply
    (λ (res) (list (first res) (third res)))
    (parse/and pOuter pInner pEnd)))

(define (parse/? p)
  (parse/or
    p
    (parse/lit "")))



(define (parse/check-char func)
  (λ (input)
    (if (equal? "" input)
      (list 'fail input)
      (begin
        (let ([first-char (string-ref input 0)])
          (if (func first-char)
            (list first-char (substring input 1))
            (list 'fail input)))))))

(define (parse/whitespace)
  (parse/check-char char-whitespace?))

(define (parse/digit)
  (parse/check-char char-numeric?))

(define (parse/strip-whitespace parser)
  (parse/between (parse/* (parse/whitespace)) parser))

(define (parse/number)
  (parse/apply
    (λ (res) (string->number (list->string res)))
    (parse/+ (parse/digit))))

