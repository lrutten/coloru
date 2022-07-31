(defn doe3 [k]
   (k)
)

(defn doe2 [k]
   (k 8)
)

(defn doe [n val]
   (doe2 
      (fn [val] 
         (doe3
            (fn []
               n
            )
         )
      )
   )
)

(println "resultaat " (doe 13 7))

