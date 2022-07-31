(defn doe2 [n k]
   (k)
)

(defn doe3 [n k]
   (let []
      (println "doe3 n " n)
      34
   )
)

(defn doe1 [n k]
   (doe2
      (- n 1)
      (fn []
         (doe3 
            (- n 2)
            (fn [] 
               (k)
            )
         )
      )
   )
)

(doe1 5 (fn [] (+ 1 2)))

