(defn doe2 [k]
   (k 3)
)

(defn doe [val]
   (doe2 (fn [v] (+ v val)))
)

(println "resultaat " (doe 7))

