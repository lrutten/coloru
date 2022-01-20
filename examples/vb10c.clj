;(defn indent [d]
;   (if (= d 0)
;     (print "")
;     (let []
;        (print "  ")
;        (indent (- d 1))
;     )
;   )
;)

(defn fib [n d k]
   ;(indent d)
   ;(println "fib " n)
   (if
      (< n 3)
      ;(let []
      ;   (indent (+ d 1))
      ;   (println "r1 1")
         (k
            1 d
         )
      ;)
      ;(let []
      ;   (indent (+ d 1))
      ;   (println "r2")
         (fib
            (- n 1)
            (+ d 1)
            (fn [value1 d]
               ;(indent (+ d 2))
               ;(println "r3 v1 " value1)
               (fib 
                  (- n 2)
                  (+ d 2)
                  (fn [value2 d] 
                     ;(indent (+ d 2))
                     ;(println "r4 v1 v2 " value1 value2)
                     (k
                       (+ value1 value2) d
                     )
                  )
               )
            )
         )
      ;)
   )
)

(let [res (fib 5 0 (fn [value d] value))]
   (println "(fib 5) is " res)
)

