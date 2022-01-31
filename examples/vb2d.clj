(defn fac
    [n]
    (println "fac " n " " (context!))
    (if
       (= n 0)
       1
       (* n (fac (- n 1)))
    )
)

(println (context!))
(fac 2)


