(defn actie [k] 
   (fn [m]
      (* m k)
   )
)

(println "res " ((actie 3) 7))

