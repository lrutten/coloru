(defn indent [d]
   (if (= d 0)
     (print "")
     (let []
        (print "   ")
        (indent (- d 1))
     )
   )
)

(defn fib [n d k]
   ;(indent d)
   (println "fib " n)
   (if
      (< n 3)
      (let []
         ;(indent (+ d 1))
         (println "r1 1")
         (k
            1
         )
      )
      (let []
         ;(indent (+ d 1))
         (println "r2")
         (fib
            (- n 1)
            (+ d 1)
            (fn [value1]
               ;(indent (+ d 2))
               (println "r3" value1)
               (fib 
                  (- n 2)
                  (+ d 2)
                  (fn [value2] 
                     ;(indent (+ d 2))
                     (println "r4" value1 value2)
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

(let [res (fib 5 0 (fn [value] value))]
   (println "(fib 5) is " res)
)

