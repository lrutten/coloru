(defn fib
    [n]
    (if
       (< n 1)
       1
       (+ (fib (- n 1)) (fib (- n 2)))
    )
)

(fib 5)



