(defn indent [d]
   (if (= d 0)
     (print "")
     (let []
        (print "  ")
        (indent (- d 1))
     )
   )
)

(defn fib [n d k]
   (indent d)
   (println "a called (fib " n ")")
   (if
      (< n 3)
      (let []
         (indent (+ d 1))
         (println "b call (k 1)")
         (k
            1 (+ d 1)
         )
      )
      (let []
         (indent (+ d 1))
         (println "c call (fib " (- n  1) "(fn [value1] (fib " (- n 2) "(fn [value2] (k (+ value1 value2)))")
         (fib
            (- n 1)
            (+ d 1)
            (fn [value1 d]
               (indent (+ d 1))
               (println "d called (fn " value1 ")")
               (indent (+ d 1))
               (println "e call (fib " (- n  2) "(fn [value2] (k (+ " value1 "value2))))")
               (fib 
                  (- n 2)
                  (+ d 2)
                  (fn [value2 d] 
                     (indent (+ d 1))
                     (println "f called (fn " value2 ")")
                     (indent (+ d 1))
                     (println "g call (k " value1 " " value2 ")")
                     (k
                       (+ value1 value2) d
                     )
                  )
               )
            )
         )
      )
   )
)

(let [res (fib 5 0 (fn [value d] value))]
   (println "(fib 5) is " res)
)

