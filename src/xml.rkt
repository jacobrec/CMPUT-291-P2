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

(define term-file (open-output-file "term.txt"))
(define (add-term type t row)
  (fprintf term-file "~a-~a:~a~%" type t row)
  (printf "~a-~a:~a~%" type t row))
(define (parse-terms ele)
  ; Get row number
  (define row
    (pcdata-string
      (first
        (element-content
          (first
            (element-content ele))))))
  (define (get-text xml)
    (apply string-append
      (for/list ([v xml])
        (string-append " " (pcdata-string v) " "))))

  ; Get subject
  (define subj
    (get-text
      (element-content
        (fifth
          (element-content ele)))))

  ; Get body
  (define body
    (get-text
      (element-content
        (eighth
          (element-content ele)))))

  ; transform and process data
  (define (finish type str)
    (set! str (string-replace str #px"[^(0-9)|(a-z)|(A-Z)|_|\\-|\\s]" " "))
    (set! str (string-downcase str))
    (for ([s (string-split str)])
      (when (> (string-length s) 2)
        (add-term type s row))))

  (finish "s" subj)
  (finish "b" body))





;; For testing
(process-mail-xml-file "10.xml"
                       (lambda (element)
                        (parse-terms element)))
