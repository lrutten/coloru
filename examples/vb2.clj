(defn fac
    [n]
    (if
       (= n 0)
       1
       (* n (fac (- n 1)))
    )
)
(defn doe [f] (f 4))
((fn [x] (* 2 x)) 4)


