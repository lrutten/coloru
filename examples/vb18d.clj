; error:
;    println wrongly finds a value for n
;    while n does not exist at that point
(defn doe2 [n k]
   (k
      1
   )
)

(defn doe3 [n]
   n
)

(defn doe1 [m k]
   (doe2
      (- m 1)
      (fn [value1]
         (doe3
            (let []
               (println "n " n)
               (- n 2)
            )
         )
      )
   )
)

(doe1 5 (fn [value] value))

