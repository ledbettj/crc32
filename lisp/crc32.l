;; build and return the crc32 table
(defun crc-init ()
  (coerce
    (loop for i from 0 below 256 collect
      (reduce
        (lambda (accum c)
          (declare  (ignore c))
          (boole boole-and #xFFFFFFFF
            (if (eq (boole boole-and accum 1) 1)
              (boole boole-xor #xEDB88320 (ash accum -1))
              (ash accum -1))))
        (loop for j from 0 below 8 collect j) ; seq
        :initial-value i))
  'vector))

;; start a new crc32 calculation and return the initial value
(defun crc-start () #xFFFFFFFF)

;; update a crc32 and return the new in-progress value
(defun crc-update (table crc bytes)
  (reduce
    (lambda (accum ch)
      (boole boole-xor
        (elt
          table
          (boole boole-and
            (boole boole-xor accum ch)
            #xFF))
        (ash accum -8)))
    bytes
    :initial-value crc))

;; finalize a crc32 calculation
(defun crc-finalize (crc)
  (boole boole-xor crc #xFFFFFFFF))

(let ((table (crc-init)))
  (loop for filename in ext:*args* do
    (let ((in (open filename
                :if-does-not-exist nil
                :element-type '(unsigned-byte 8)))
           (seq (make-array (* 1024 1024)
                  :element-type '(unsigned-byte 8)
                  :adjustable t
                  :fill-pointer (* 1024 1024)))
           (crc (crc-start)))
      (when in
        ;; read file and update crc value
        (progn
          (loop
            (setf (fill-pointer seq) (read-byte-sequence seq in))
            (when (zerop (fill-pointer seq))
              (return))
            (setq crc (crc-update table crc seq)))
          (format t "~A: ~X~%" filename (crc-finalize crc)))))))
