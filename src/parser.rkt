#lang racket

(require "parsertools.rkt")
(provide command)

#|
alphanum     ::= [0-9a-zA-Z_-]
num          ::= [0-9]

date         ::= num num num num '/' num num '/' num num
datePrefix   ::= 'date' whitespace* (':' | '>' | '<' | '>=' | '<=')
dateQuery    ::= datePrefix whitespace* date

emailterm    ::= alphanum+ | alphanum+ '.' emailterm
email        ::= emailterm '@' emailterm
emailPrefix  ::= (from | to | cc | bcc) whitespace* ':'
emailQuery   ::= emailPrefix whitespace* email

term         ::= alphanum+
termPrefix   ::= (subj | body) whitespace* ':'
termSuffix   ::= '%'
termQuery    ::= termPrefix? whitespace* term termSuffix?

expression   ::= dateQuery | emailQuery | termQuery
query        ::= expression (whitespace expression)*

modeChange   ::= 'output=full' | 'output=brief'

command      ::= query | modeChange
|#

(define alpha
  (parse/or-lit
    "a" "b" "c" "d" "e" "f" "g" "h" "i" "j" "k" "l" "m"
    "n" "o" "p" "q" "r" "s" "t" "u" "v" "w" "x" "y" "z"
    "A" "B" "C" "D" "E" "F" "G" "H" "I" "J" "K" "L" "M"
    "N" "O" "P" "Q" "R" "S" "T" "U" "V" "W" "X" "Y" "Z"))

(define num
  (parse/or-lit
    "0" "1" "2" "3" "4" "5" "6" "7" "8" "9"))

(define alphanum
  (parse/or
    alpha
    num
    (parse/lit "_")
    (parse/lit "-")))

(define alphanumstring
  (parse/apply
    (λ (x) (apply string-append x))
    (parse/+ alphanum)))

(define date
  (parse/apply
    (λ (datechars) ; Dates will be stored as strings
      (apply string-append datechars))

    (parse/and num num num num
               (parse/lit "/")
               num num
               (parse/lit "/")
               num num)))

(define dateprefix
  (parse/around
    (parse/lit "date")
    (parse/* (parse/whitespace))
    (parse/or-lit ":" ">=" "<=" ">" "<")))

(define datequery
  (parse/apply
    (λ (data)
       (list 'date (cadar data) (cadr data)))
    (parse/around
      dateprefix
      (parse/* (parse/whitespace))
      date)))


(define (emailterm)
  (λ (input)
     ((parse/or
        (parse/apply
          (λ (x)
             (apply string-append x))
          (parse/and alphanumstring (parse/lit ".") (emailterm)))
        alphanumstring)
      input)))

(define email
  (parse/apply
    (λ (x) (apply string-append x))
    (parse/and
      (emailterm)
      (parse/lit "@")
      (emailterm))))

(define emailprefix
  (parse/apply car
    (parse/around
      (parse/or-lit "from" "to" "cc" "bcc")
      (parse/* (parse/whitespace))
      (parse/lit ":"))))

(define emailquery
  (parse/apply
    (λ (x)
       (cons 'email x))
    (parse/around
      emailprefix
      (parse/* (parse/whitespace))
      email)))

(define term
  alphanumstring)

(define termprefix
  (parse/apply
    (λ (x)
       (car x))
    (parse/and
      (parse/or-lit "subj" "body")
      (parse/* (parse/whitespace))
      (parse/lit ":"))))

(define termsuffix
  (parse/lit "%"))

(define termquery
  (parse/apply
    (λ (x)
       (list 'term
             (cond
               [(string=? (car x) "subj") 'subj]
               [(string=? (car x) "body") 'body]
               [(string=? (car x) "") 'both])
             (caddr x) (string=? "%" (cadddr x))))
    (parse/and
      (parse/? termprefix)
      (parse/* (parse/whitespace))
      term
      (parse/? termsuffix))))

(define expression
  (parse/or
    datequery
    emailquery
    termquery))

(define query
  (parse/apply
    (λ (x) (cons 'query x))
    (parse/+
      (parse/apply car
        (parse/and
          expression
          (parse/* (parse/whitespace)))))))

(define modechange
  (parse/apply
    (λ (x) (list 'mode (if (string=? x "output=full") 'full 'brief)))
    (parse/or-lit
      "output=full"
      "output=brief")))

(define command
  (parse/or
    modechange
    query))

#|
(command "subj:gas body:earning")
(command "bcc:testy@test.ca")
(command "date>1998/12/08")
(command "output=full")
(command "output=brief")
|#
