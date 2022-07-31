(defn actie [n fu] 
   (fu n)
)

(actie 
   3 
   (fn [v] 
      (* 2 v)
   )
)

