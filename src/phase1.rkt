#lang racket

(require "xml.rkt")



;;; This does phase 1, assuming the file is coming from standard in
(process-mail-xml-file
  (current-input-port)
  phase1-process-elements)
