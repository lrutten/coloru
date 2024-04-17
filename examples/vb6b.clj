(defn lijst [] '(11 22 33))

(defn tel [ [a & b] ]
   (if (nil? a)
      (println "nil")
      (if (empty? b)
         (println "empty")
         (println "not empty")
      )
   )
)

(tel '(10 22))


