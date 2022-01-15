(defn doe2 [m fu]
   (let [n 8]
      (println m)
      (println n)
      (fu 9)
   )
)

(defn doe [k]
   (let [value1 7]
      (println k)
      (println value1)
      (doe2 6 (fn [s] (println "[" value1 "]")))
   )
)

(doe 5)


