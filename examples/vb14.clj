(defn doe [db]
   (fn [v] (db v))
)

((doe (fn [t] (+ t t))) 7)


