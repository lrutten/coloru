(defn rec
    [n]
    (if
       (= n 1)
       1
       (rec (- n 1))
    )
)

(rec 50)



