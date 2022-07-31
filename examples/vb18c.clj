; The fn takes the value of doe2 [n] for binding its n.
; This is an error.

(defn doe2 [n k]
   (k
      1
   )
)

(defn doe3 [n]
   n
)

(defn doe1 [n k]
   (doe2
      (- n 1)
      (fn [value1]
         (doe3 
            (- n 2)
         )
      )
   )
)

(doe1 5 (fn [value] value))

