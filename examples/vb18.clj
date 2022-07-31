(defn doe2 [n k]
   (let []
      (println "doe2 n " n)
      (k
         1
      )
   )
)

(defn doe3 [n k]
   (println "doe3 n " n)
)

(defn doe1 [n k]
   (let []
      (println "doe1 n " n)
      (doe2
         (- n 1)
         (fn [value1]
            (let []
               (println "fna n " n)
               (doe3 
                  (- n 2)
                  (fn [value2] 
                     (let []
                        (println "fnb n " n)
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
)

(doe1 5 (fn [value] value))

