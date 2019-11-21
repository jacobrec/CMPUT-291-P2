#lang racket
(require xml)
(provide process-mail-xml-file
         phase1-process-elements)

(define path-prefix "output/")
(define rec-file
  (open-output-file
    (string-append path-prefix "recs.txt")
    #:exists 'replace))
(define date-file
  (open-output-file
    (string-append path-prefix "dates.txt")
    #:exists 'replace))
(define email-file
  (open-output-file
    (string-append path-prefix "emails.txt")
    #:exists 'replace))
(define term-file
  (open-output-file
    (string-append path-prefix "terms.txt")
    #:exists 'replace))

;; Takes in a file, and a function
; It will process the elements in the xml file one at a time
; Calling that function on each of them
(define (process-mail-xml-file in fn)
  (define doctype (read-line in))
  (define email-array (read-line in))
  (with-handlers ([exn:xml? (lambda (exn)
                               (void))])
    (define (loop)
      (fn (read-xml/element in))
      (loop))
    (loop)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the recs file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (add-rec t row)
  (fprintf date-file "~a:~a~%" row t))
(define (parse-recs ele)
  (define row (get-row ele))
  (display row rec-file)
  (display ":" rec-file)

  (define xml-str (open-output-string))
  (write-xml/content ele xml-str)
  (define s (get-output-string xml-str))

  (set! s (string-replace s #px"\n" "&#10;"))
  (set! s (string-replace s #px"'" "&apos;"))
  (set! s (string-replace s #px"\"" "&quot;"))

  (displayln s rec-file))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the date file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (add-date t row)
  (fprintf date-file "~a:~a~%" t row))
(define (parse-dates ele)
  (define row (get-row ele))

  (define date (get-item-from-email ele second))
  (add-date date row))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the email file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (add-email type t row)
  (fprintf email-file "~a-~a:~a~%" type t row))
(define (parse-emails ele)
  (define row (get-row ele))
  (define (do-email-type type loc-th)
    (define t-email (get-item-from-email ele loc-th))
    (when (non-empty-string? t-email)
      (for ([e (string-split t-email ",")])
        (add-email type e row))))

  (do-email-type "from" third)
  (do-email-type "to" fourth)
  (do-email-type "cc" sixth)
  (do-email-type "bcc" seventh))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This makes the term file ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
    (set! str
      (string-replace str #px"[\\(|\\)]" " "))
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
        (cond
          [(pcdata? v) (string-append " " (pcdata-string v) " ")]
          [(entity? v) (string-append " " (string (integer->char (entity-text v))) " ")])))))

(define (phase1-process-elements element)
  (parse-terms element)
  (parse-emails element)
  (parse-dates element)
  (parse-recs element))


