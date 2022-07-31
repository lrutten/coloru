(defn fibc [k]
   (k
      10
   )
)

(defn fibb [k]
   (k
      20
   )
)

(defn fiba [k]
   (fibb
      (fn [value1]
         (fibc 
            (fn [value2] 
               (k
                 (+ value1 value2)
               )
            )
         )
      )
   )
)

(fiba (fn [value] value))

