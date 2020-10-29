(defn lijst [] '(11 22 33))

(defn tel [[a & b]]
   (if (nil? a)
      0
      (if (empty? b)
         1
         (+ 1 (tel b))
      )
   )
)


'(tel '(10 20 30))
(println (tel (lijst)))


