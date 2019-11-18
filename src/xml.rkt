#lang racket
(require xml)

;; Takes in a file, and a function
; It will process the elements in the xml file one at a time
; Calling that function on each of them
(define (process-mail-xml-file file fn)
  (define in (open-input-file file))
  (define doctype (read-line in))
  (define email-array (read-line in))
  (with-handlers ([exn:xml? (lambda (exn)
                               (void))])
    (define (loop)
      (fn (read-xml/element in))
      (loop))
    (loop)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the date file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define date-file (open-output-file "dates.txt" #:exists 'replace))
(define (add-date t row)
  (fprintf date-file "~a:~a~%" t row))
(define (parse-dates ele)
  (define row (get-row ele))

  (define date (get-item-from-email ele second))
  (add-date date row))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the email file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define email-file (open-output-file "emails.txt" #:exists 'replace))
(define (add-email type t row)
  (fprintf email-file "~a-~a:~a~%" type t row))
(define (parse-emails ele)
  (define row (get-row ele))
  (define (do-email-type type loc-th)
    (define t-email (get-item-from-email ele loc-th))
    (when (non-empty-string? t-email)
      (add-email type t-email row)))

  (do-email-type "from" third)
  (do-email-type "to" fourth)
  (do-email-type "cc" sixth)
  (do-email-type "bcc" seventh))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the term file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define term-file (open-output-file "terms.txt" #:exists 'replace))
(define (add-term type t row)
  (fprintf term-file "~a-~a:~a~%" type t row))
(define (parse-terms ele)
  (define row (get-row ele))
  (define subj (get-item-from-email ele fifth))
  (define body (get-item-from-email ele eighth))

  ; transform and process data
  (define (finish type str)
    (set! str
      (string-replace str #px"[^(0-9)|(a-z)|(A-Z)|_|\\-|\\s]" " "))
    (set! str (string-downcase str))
    (for ([s (string-split str)])
      (when (> (string-length s) 2)
        (add-term type s row))))

  (finish "s" subj)
  (finish "b" body))



; Get nth xml text
(define (get-item-from-email ele loc-th)
  (get-text
    (element-content
      (loc-th
        (element-content ele)))))

; Get row number
(define (get-row email-xml)
  (get-item-from-email email-xml first))

; parses together several pcdata's into one string
(define (get-text xml)
  (string-trim
    (apply string-append
      (for/list ([v xml])
        (string-append " " (pcdata-string v) " ")))))


;; For testing
(process-mail-xml-file "10.xml"
                       (lambda (element)
                        (parse-terms element)
                        (parse-emails element)
                        (parse-dates element)))
