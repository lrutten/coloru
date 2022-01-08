(defn fib [n k]
   (if
      (< n 1 )
      (k
         1
      )
      (fib
         (- n 1 )
         (fn
            [value ]
               (k
                  (+ value (fib (- n 2 )))
               )
         )
      )
   )
)

(fib
   4
   (fn
      [value ]
      value
   )
)

