(defn tel [a]
   (println a)
   (tel (+ 1 a))
)

(println (tel 0))


