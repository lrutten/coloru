(defn doe3 [k]
   (k)
)

(defn doe2 [k]
   (k 8)
)

(defn doe [val]
   (doe2 
      (fn [val2] 
         (doe3
            (fn []
               val2
            )
         )
      )
   )
)

(println "resultaat " (doe 7))

