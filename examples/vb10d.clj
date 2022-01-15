(defn fib [n k]
   (if
      (< n 3)
      (k
         1
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

(let [res (fib 5 (fn [value] value))]
   (println "(fib 5) is " res)
)

