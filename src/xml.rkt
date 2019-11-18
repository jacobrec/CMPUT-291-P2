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


;; For testing
(process-mail-xml-file "10.xml" displayln)
