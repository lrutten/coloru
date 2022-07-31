(defn fib [n k]
   (if
      (< n 3)
      (k
         1
      )
      (fib
         (- n 1)
         (fn [v]
            (fib 
               (- n 2)
               (fn [w] 
                  (k
                    (+ v w)
                  )
               )
            )
         )
      )
   )
)

(fib 5 (fn [val] val))

