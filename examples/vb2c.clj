(defn fac
    [n]
    (if
       (= n 0)
       1
       (* n (fac (- n 1)))
    )
)

(println "fac 5 " (fac 5))


