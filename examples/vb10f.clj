; This version returns 5 which is correct

(defn fib [n k]
   (if
      (< n 3)
      (k
         1
      )
      (if
         (< n 4)
         (k
            2
         )
         (fib
            (- n 1)
            (fn [value1]
               (fib 
                  (- n 2)
                  (fn [value2] 
                     (k
                       (+ value1 value2)
                     )
                  )
               )
            )
         )
      )
   )
)

(println "(fib 5) is " (fib 5 (fn [value] value)))
